#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <math.h>

// Defines
#define     TXT_WIDTH   100
#define     XY_FACTOR   2

using namespace cv;
using namespace std;

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
}

void process_img(char* full_path, char * img_filename)
{
    Mat image;
    image = imread(full_path);
    //imshow("orig", image);
    image.convertTo(image, -1, 1, 100);
    //imshow("satur", image);

    int txt_height = (image.size[0] * 100 / image.size[1])/XY_FACTOR;
    Size  sz = Size(TXT_WIDTH, txt_height);

    cvtColor(image, image, CV_BGR2GRAY);

    blur(image, image, Size(7,7) );

    /*
    // Checking results
    image = image + edges;
    imshow("edges", edges);
    imshow("sum", image);
    */

    equalizeHist(image, image);
    resize(image, image, sz, 0, 0);
    //Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    char outfilename[500] = {0};
    sprintf(outfilename, "/home/noureddine-as/CLionProjects/img2txt/cmake-build-debug/output/%s_out.txt", img_filename);
    do_lookup(image, "8?!:,. ", outfilename);

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
