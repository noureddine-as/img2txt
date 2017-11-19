#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <math.h>

// Defines
#define     TXT_WIDTH   100
//#define     TXT_HEIGHT  35

using namespace cv;
using namespace std;

void drawText(Mat & image);
void do_lookup(Mat& src, const char* code, char* fn);
void process_img(char* full_path, char * img_filename);


int main()
{
    cout << "Built with L<3VE by Noureddine-as"<< endl;

    //image = imread("/home/noureddine-as/hacker.jpeg");
    //cout << (int)argc << "\n" << argv[1] ;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/home/noureddine-as/CLionProjects/img2txt/cmake-build-debug/images")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_REG)
            {
                //printf ("%s\n", ent->d_name);
                // Here are the valid files
                // Files must be valid images
                char full_path[500] = {0};
                sprintf(full_path, "/home/noureddine-as/CLionProjects/img2txt/cmake-build-debug/images/%s", ent->d_name);
                cout << full_path << endl;
                //image = imread(full_path);
                //imshow("hh", image);
                process_img(full_path, ent->d_name);
                //waitKey(0);

            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }

return 0;
    //cout << image.size << endl ;
    //imshow("Hello", image);



    /*
    VideoCapture capture;
    capture.open(0);
    if(capture.isOpened())
    {
        cout << "Capture is opened" << endl;
        for(;;)
        {
            capture >> image;
            if(image.empty())
                break;
            drawText(image);
            imshow("Sample", image);
            if(waitKey(10) >= 0)
                break;
        }
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        drawText(image);
        imshow("Sample", image);
        waitKey(0);
    }
    */
    return 0;
}

void drawText(Mat & image)
{
    putText(image, "Hello OpenCV",
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}

void process_img(char* full_path, char * img_filename)
{
    Mat image;
    image = imread(full_path);
    imshow("orig", image);
    image.convertTo(image, -1, 1, 80);
    imshow("satur", image);


    int txt_height = (image.size[0] * 100 / image.size[1])/2;
    Size  sz = Size(TXT_WIDTH, txt_height);

    //cout << sz << endl ;
    resize(image, image, sz, 0, 0);
    cvtColor(image, image, CV_BGR2GRAY);
    equalizeHist(image, image);


    char outfilename[500] = {0};
    sprintf(outfilename, "/home/noureddine-as/CLionProjects/img2txt/cmake-build-debug/output/%s_out.txt", img_filename);
    do_lookup(image, "8?!:,. ", outfilename);
//    do_lookup(image, "88000000005", outfilename);

    //waitKey(0);
}

void do_lookup(Mat& src, const char* code, char* fn)
{
    ofstream fo;
    fo.open(fn);

    uchar lookuptable[256] = {0};
    for(int i=0; i<strlen(code); i++)
    {
        for(int j=(256/strlen(code))*i; j<MAX((256/strlen(code))*(i+1), 256); j++){
            lookuptable[j] = (uchar)code[i];
            //cout << j << "\"" << lookuptable[j] << "\"" << endl;
        }
    }

    MatIterator_<uchar> it, end;
    for(it=src.begin<uchar>(), end=src.end<uchar>(); it != end; ++it)
    {
        fo << (char)lookuptable[(*it)];

        if((it!= src.begin<uchar>()) && ((it - src.begin<uchar>() +1)%TXT_WIDTH == 0))
            fo << "\n";
    }
    fo.close();
}
