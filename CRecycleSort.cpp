/*
	* MIT License
 * 
 * Copyright (c) 2025 jasmine-portfolio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* ----------------------------------------------
   Title: CRecycleSort.cpp
   Description: 
     This file contains the implementation of the CRecycleSort class,
     which is responsible for controlling the recycling sorting process
     using OpenCV for image processing and pigpio for interacting with
     GPIO pins on the Raspberry Pi. It includes methods for detecting
     colors in the video stream, controlling servos and LEDs, and
     managing the sorting process.
     
   Author: Jasmine
   Date: March 23, 2023

   Additional Notes:
     - Make sure that the pigpio library is running on the Raspberry Pi.
     - The OpenCV library and cvui must be correctly installed.
   ----------------------------------------------*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include "cvui.h"
#include "CRecycleSort.h"
#include "pigpio.h"


// GPIO Pin Definitions
#define CANVAS_NAME "Ball Sorter"
#define CVUI_IMPLEMENTATION
#define ON_OFF 17
#define PIN_SERVO1 12
#define PIN_SERVO2 26

#define LED_BLUE 16
#define LED_GREEN 20
#define LED_RED 21
#define LED_WHITE 24
#define LED_YELLOW 23

#define SERVO_BUCKET_UP 1200
#define SERVO_BUCKET_DOWN 1600

#define ANGLE_BIN_WHITE 1000
#define ANGLE_BIN_GREEN 1250
#define ANGLE_BIN_RED 1500
#define ANGLE_BIN_BLUE 1750

// Constructor for CRecycleSort class
CRecycleSort::CRecycleSort() {
    // Initialize variables with default values
    angle = SERVO_BUCKET_UP; 
    color_name = 'L';
    previous_color = 'L';
    do_exit = false;
    on_off = 0;
    bin_num = 1;
    pixel_hue = 0;
    pixel_sat = 0;
    pixel_value = 0;
    Blue = 0;
    Green = 0;
    White = 0;
    Pink = 0;
}

// Destructor for CRecycleSort class
CRecycleSort::~CRecycleSort() {
    // Destructor (currently no specific clean-up)
}

// Function to initialize the GPIO pins
void CRecycleSort::Initialize_gpio() {
    if (gpioInitialise() < 0) {
        std::cout << "Error initializing GPIO" << std::endl;
    }

    // Setting up GPIO pins for input and output
    gpioSetMode(PIN_SERVO1, PI_OUTPUT);  // Servo 1 output
    gpioSetMode(PIN_SERVO2, PI_OUTPUT);  // Servo 2 output
    gpioSetMode(LED_BLUE, PI_OUTPUT);   // LED Blue output
    gpioSetMode(LED_GREEN, PI_OUTPUT);  // LED Green output
    gpioSetMode(LED_RED, PI_OUTPUT);    // LED Red output
    gpioSetMode(LED_WHITE, PI_OUTPUT);  // LED White output
    gpioSetMode(LED_YELLOW, PI_OUTPUT); // LED Yellow output
}

// Function to update the system based on the captured frame
void CRecycleSort::update() {
    // Convert the frame to HSV color space
    cv::cvtColor(frame, frame_HSV, cv::COLOR_BGR2HSV);
    
    // Calculate average pixel hue, saturation, and value in a specific region
    for (int i = 255; i < 390; i++) {
        for (int j = 205; j < 350; j++) {
            pixel_hue += frame_HSV.at<cv::Vec3b>(i, j)[0];
            pixel_sat += frame_HSV.at<cv::Vec3b>(i, j)[1];
            pixel_value += frame_HSV.at<cv::Vec3b>(i, j)[2];
        }
    }

    // Average pixel values
    pixel_hue = pixel_hue / 19575;
    pixel_sat = pixel_sat / 19575;
    pixel_value = pixel_value / 19575;

    // Determine color based on pixel values
    if (0 <= pixel_value && pixel_value <= 70) {
        color_name = "No ball";
    } else if (0 <= pixel_sat && pixel_sat <= 68) {
        color_name = "White";
    } else if ((45 <= pixel_hue && pixel_hue <= 74)) {
        color_name = "Green";
    } else if ((120 <= pixel_hue && pixel_hue <= 165)) {
        color_name = "Pink";
    } else if ((75 <= pixel_hue && pixel_hue <= 110)) {
        color_name = "Blue";
    } else {
        color_name = "Unknown color";
    }

    // Handling of servo motor and LED switches based on current state
    if ((on_off == 2) || (gpioRead(ON_OFF) == 1)) {
        gpioWrite(LED_YELLOW, 1); // Yellow LED
        elapsed_time = (cv::getTickCount() - start_tic) / freq;
        
        if (elapsed_time > 1) {
            // Toggle servo between two positions
            if (angle == SERVO_BUCKET_UP) {
                gpioServo(ON_OFF, SERVO_BUCKET_DOWN);
                temp_angle = SERVO_BUCKET_DOWN;
            }
            if (angle == SERVO_BUCKET_DOWN) {
                gpioServo(ON_OFF, SERVO_BUCKET_UP);
                temp_angle = SERVO_BUCKET_UP;
                previous_color = color_name;
            }
            angle = temp_angle;
            start_tic = cv::getTickCount();

            if (angle == SERVO_BUCKET_UP) {
                // Sort the color and increment the respective count
                if (previous_color[0] == 'W') {
                    Select_Bin(ANGLE_BIN_WHITE, 'W');
                    White++;
                } else if (previous_color[0] == 'G') {
                    Select_Bin(ANGLE_BIN_GREEN, 'G');
                    Green++;
                } else if (previous_color[0] == 'P') {
                    Select_Bin(ANGLE_BIN_RED, 'R');
                    Pink++;
                } else if (previous_color[0] == 'B') {
                    Select_Bin(ANGLE_BIN_BLUE, 'B');
                    Blue++;
                }
            }
        }
    }
}

// Function to draw GUI elements (using cvui library)
void CRecycleSort::draw() {
    // GUI Menu (using cvui library)
    gui_position = cv::Point(10, 10);
    cvui::window(frame, gui_position.x, gui_position.y, 200, 265, "Sorting     Color: " + color_name);

    // Drawing the color detection area
    cv::rectangle(frame, cv::Point(255, 205), cv::Point(390, 350), cv::Scalar(200, 200, 200), 2);
    gui_position += cv::Point(0, 25);
    cvui::text(frame, gui_position.x, gui_position.y, "State: Switch    CVUI    Auto");

    // Trackbars for controlling the state and bin number
    gui_position += cv::Point(50, 10);
    cvui::trackbar(frame, gui_position.x, gui_position.y, 150, &on_off, 0, 2);
    gui_position += cv::Point(-50, 55);
    cvui::text(frame, gui_position.x, gui_position.y, "Bin:");
    gui_position += cv::Point(50, 0);
    cvui::trackbar(frame, gui_position.x, gui_position.y, 150, &bin_num, 1, 4);

    gui_position += cv::Point(-40, 50);
    if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit")) {
        do_exit = true;
    }

    gui_position += cv::Point(0, 35);
    cvui::text(frame, gui_position.x, gui_position.y, "1-White: " + std::to_string(White) + "  2-Green: " + std::to_string(Green));
    gui_position += cv::Point(0, 25);
    cvui::text(frame, gui_position.x, gui_position.y, "3-Pink: " + std::to_string(Pink) + "  4-Blue: " + std::to_string(Blue));
    gui_position += cv::Point(0, 25);
    
    // Reset Counter Button
    if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Reset Counter")) {
        White = 0;
        Green = 0;
        Pink = 0;
        Blue = 0;
    }

    // Logic for controlling LEDs and servo based on on_off state
    if (((on_off == 0) || (on_off == 1)) && (gpioRead(ON_OFF) == 0)) {
        gpioWrite(LED_YELLOW, 0); // Yellow LED off

        if (on_off == 1) {
            gui_position += cv::Point(110, 0);
            if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Drop")) {
                // Handle drop button logic
                if (angle == SERVO_BUCKET_UP) {
                    gpioServo(PIN_SERVO1, SERVO_BUCKET_DOWN); 
                    temp_angle = SERVO_BUCKET_DOWN;
                }
                if (angle == SERVO_BUCKET_DOWN) {
                    gpioServo(PIN_SERVO1, SERVO_BUCKET_UP);
                    temp_angle = SERVO_BUCKET_UP;
                    previous_color = color_name;
                }
                angle = temp_angle;
                if (angle == SERVO_BUCKET_UP) {
                    if (previous_color[0] == 'W') {
                        White++;
                    } else if (previous_color[0] == 'G') {
                        Green++;
                    } else if (previous_color[0] == 'P') {
                        Pink++;
                    } else if (previous_color[0] == 'B') {
                        Blue++;
                    }
                }
            }

            // Control bins based on selected bin number
            if (bin_num == 1) {
                Select_Bin(ANGLE_BIN_WHITE, 'W');
            } else if (bin_num == 2) { 
                Select_Bin(ANGLE_BIN_GREEN, 'G');
            } else if (bin_num == 3) {
                Select_Bin(ANGLE_BIN_RED, 'R');
            } else if (bin_num == 4) {
                Select_Bin(ANGLE_BIN_BLUE, 'B');
            }
        }
    }

    // Update the GUI and display the frame
    cvui::update();
    cv::imshow(CANVAS_NAME, frame);
}

// Function to select the bin based on the color
void CRecycleSort::Select_Bin(int angle, char color_bin) {
    gpioServo(PIN_SERVO2, angle); 

    if(color_bin == 'B'){
        gpioWrite(LED_BLUE, 1); 
        gpioWrite(LED_GREEN, 0); 
        gpioWrite(LED_RED, 0); 
        gpioWrite(LED_WHITE, 0); 
    } else if(color_bin == 'G'){
        gpioWrite(LED_BLUE, 0); 
        gpioWrite(LED_GREEN, 1); 
        gpioWrite(LED_RED, 0); 
        gpioWrite(LED_WHITE, 0); 
    } else if(color_bin == 'R'){
        gpioWrite(LED_BLUE, 0); 
        gpioWrite(LED_GREEN, 0); 
        gpioWrite(LED_RED, 1); 
        gpioWrite(LED_WHITE, 0); 
    } else if(color_bin == 'W'){
        gpioWrite(LED_BLUE, 0); 
        gpioWrite(LED_GREEN, 0); 
        gpioWrite(LED_RED, 0); 
        gpioWrite(LED_WHITE, 1); 
    }
}



// Main run function for controlling the entire system
void CRecycleSort::run() {
    // Open video capture
    cv::VideoCapture vid;
    vid.open(0, cv::CAP_V4L);

    // Initialize the GUI system and GPIO
    cvui::init(CANVAS_NAME);
    Initialize_gpio();
    freq = cv::getTickFrequency();
    start_tic = cv::getTickCount();

    if (vid.isOpened() == true) {
        do {
            // Capture video frame
            vid >> frame;

            // Check if frame is valid
            if (frame.empty() == false) {
                update();  // Process frame
                draw();    // Render the GUI
            }
        } while (cv::waitKey(1) != 'q' && do_exit == false);  // Exit on 'q' key press
    }

    // Terminate GPIO setup when done
    gpioTerminate();
}
