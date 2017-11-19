#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <dirent.h>

// Defines
#define     TXT_WIDTH   120
//#define     TXT_HEIGHT  35

using namespace cv;
using namespace std;

void drawText(Mat & image);
void do_lookup(Mat& src, const char* code, char* fn);

int main(char argc, char** argv)
{
    cout << "Built with L<3VE by Noureddine-as"<< endl;
    Mat image;

    //image = imread("/home/noureddine-as/hacker.jpeg");
    //cout << (int)argc << "\n" << argv[1] ;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/home/noureddine-as/CLionProjects/img2txt/cmake-build-debug/images")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_REG)
                printf ("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }

    return 0;




    image = imread(argv[1]);

    cout << image.size << endl ;
    //imshow("Hello", image);

    int txt_height = (image.size[0] * 100 / image.size[1]);
    Size  sz = Size(TXT_WIDTH, txt_height);

    cout << sz << endl ;
    resize(image, image, sz, 0, 0);
    cvtColor(image, image, CV_BGR2GRAY);
    equalizeHist(image, image);

    //threshold(image, image, 255/2.0, 255, THRESH_BINARY);

    //cout << image;
    //do_lookup(image, "OOoo,._");
    do_lookup(image, "880?!:,.", argv[1]);

    waitKey(0);


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

void do_lookup(Mat& src, const char* code, char* fn)
{
    ofstream fo;
    char filename[100] = {0};
    sprintf(filename, "images/%s_out.txt", fn);
    fo.open (filename);
    //fo << "Writing this to a file.\n";
    //myfile.close();
    //Mat lookuptable(1, 256, CV_8S);

    //Mat ret(src.size[0], src.size[1], CV_8S);
    uchar lookuptable[256] = {0};
    for(int i=1; i<strlen(code); i++)
    {
        for(int j=(255/strlen(code))*(i-1); j<(255/strlen(code))*i; j++)
        {
            lookuptable[j] = (uchar)code[i];
        }
    }
    //LUT(src, lookuptable, ret);

    MatIterator_<uchar> it, end;
    for(it=src.begin<uchar>(), end=src.end<uchar>(); it != end; ++it)
    {
        //cout << (char)lookuptable[(*it)];
        fo << (char)lookuptable[(*it)];

        if((it-src.begin<uchar>())%TXT_WIDTH == 0)
            fo << "\n";
    }
    fo.close();
}
