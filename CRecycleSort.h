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
#ifndef CRECYCLESORT_H
#define CRECYCLESORT_H
#include <string>
#include <opencv2/opencv.hpp>

class CRecycleSort
{
    public:
        CRecycleSort();  // Constructor
        virtual ~CRecycleSort();  // Destructor
        void run();  // Main function to run the recycling sorting process

    protected:
        void draw();  // Function to draw GUI and display information
        void update();  // Function to update sorting logic based on the video feed

    private:
	void Initialize_gpio();
	void Select_Bin(int angle, char color_bin)
		
        bool do_exit;  // Flag to exit the program
        double start_tic, freq, elapsed_time;  // Timer variables
        int angle, temp_angle;  // Servo angle tracking
	int on_off;  // Variables for color and sorting settings
	int bin_num;  // Variables for color thresholds
        int pixel_hue, pixel_sat, pixel_value;  // Variables for pixel color information
        int Blue, Green, White, Pink;  // Color counters
        std::string color_name;  // Name of detected color
        std::string previous_color;  // Previous detected color
	cv::Mat frame, frame_HSV;  // Matrices for video frame and processing
        cv::Point gui_position;  // Position of GUI elements

};

#endif // CRECYCLESORT_H
