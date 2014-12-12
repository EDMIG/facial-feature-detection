/*
 * Author: Samyak Datta (datta[dot]samyak[at]gmail.com)
 *
 * A program to automatically detect eyebrows and eyebrow key points using a color-based
 * method of eyebrow segmentation that extracts a pseudo-hue plane to separate the eyebrow 
 * from the skin region. 
 *
 * This code is an implementation of the following paper
 * Majumder, A. Singh, M. and Behera, L., "Automatic eyebrow features detection and realization 
 * of avatar for real time eyebrow movement", 7th IEEE International Conference on 
 * Industrial and Information Systems (ICIIS),2012
 */

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

void help();
void detectFaces(Mat&, vector<Rect_<int> >&, string);
void detectEyes(Mat&, const vector<Rect_<int> >, string);

string input_image_path;
string face_cascade_path, eye_cascade_path;

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        help();
        return 1;
    }

    input_image_path = argv[1];
    face_cascade_path = argv[2];
    eye_cascade_path = argv[3];

    // Load image
    Mat image = imread(input_image_path);

    // Detect faces and eyes in image
    vector<Rect_<int> > faces;
    detectFaces(image, faces, face_cascade_path);
    detectEyes(image, faces, eye_cascade_path);

    // Display result
    imshow("Result", image);

    waitKey(0);
    return 0;
}

void help()
{
    cout << "\nThis program demonstrates eyebrow and eyebrow key-point detection using a color-based\n"
        "method of eyebrow segmentation that extracts a hue-plane to separate\n" 
        "the eyebrow from the skin region.\n";

    cout << "\nUSAGE: ./eyebrow [IMAGE] [FACE_CASCADE] [EYE_CASCADE]\n"
        "IMAGE\n\tPath to the image of a face taken as input.\n"
        "FACE_CASCSDE\n\t Path to a haarcascade classifier for face detection.\n"
        "EYE_CASCSDE\n\t Path to a haarcascade classifier for eye detection.\n";

}

void detectFaces(Mat& img, vector<Rect_<int> >& faces, string face_cascade_path)
{
    CascadeClassifier face_cascade;
    face_cascade.load(face_cascade_path);

    face_cascade.detectMultiScale(img, faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}

void detectEyes(Mat& img, const vector<Rect_<int> > faces, string eye_cascade_path)
{
    for(unsigned int i = 0; i < faces.size(); ++i)
    {
        Rect face = faces[i];
        rectangle(img, Point(face.x, face.y), Point(face.x+face.width, face.y+face.height),
                Scalar(255, 0, 0), 1, 4);
        Mat ROI = img(Rect(face.x, face.y, face.width, face.height));

        vector<Rect_<int> > eyes;
        CascadeClassifier eyes_cascade;
        eyes_cascade.load(eye_cascade_path);

        eyes_cascade.detectMultiScale(ROI, eyes, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
        for(unsigned int j = 0; j < eyes.size(); ++j)
        {
            Rect e = eyes[j];
            // circle(ROI, Point(e.x+e.width/2, e.y+e.height/2), 3, Scalar(0, 255, 0), -1, 8);
            rectangle(ROI, Point(e.x, e.y), Point(e.x+e.width, e.y+e.height),
                Scalar(0, 255, 0), 1, 4);
        }
    }
}
