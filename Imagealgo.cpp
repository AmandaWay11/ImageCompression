#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

std::vector<unsigned char> rle_encode(cv::Mat img) {
    std::vector<unsigned char> encoded;
    int count = 1;
    unsigned char last = img.at<unsigned char>(0, 0);
    for (int i = 1; i < img.total(); i++) {
        unsigned char current = img.at<unsigned char>(i);
        if (current == last) {
            count++;
        } else {
            encoded.push_back(last);
            encoded.push_back(count);
            last = current;
            count = 1;
        }
    }
    encoded.push_back(last);
    encoded.push_back(count);
    return encoded;
}

cv::Mat rle_decode(std::vector<unsigned char> encoded, int rows, int cols) {
    cv::Mat img(rows, cols, CV_8UC1);
    int i = 0;
    int count = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (count == 0) {
                img.at<unsigned char>(r, c) = encoded[i++];
                count = encoded[i++];
            }
            count--;
        }
    }
    return img;
}

int main() {
    cv::Mat img = cv::imread("image.jpg", cv::IMREAD_GRAYSCALE);
    std::vector<unsigned char> encoded = rle_encode(img);
    cv::Mat decoded = rle_decode(encoded, img.rows, img.cols);
    cv::imwrite("compressed.bin", encoded);
    cv::imwrite("decompressed.jpg", decoded);
    return 0;
}
