#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

// RGB is : 20 255 10

int *find_pixel(unsigned char *template, unsigned char *image, int t_width, int t_height, int t_channel, 
    int i_width, int i_height, int i_channel, int initial_x, int initial_y)
{
    int *result = malloc(sizeof(int) * 2);
    result[0] = 0;
    result[1] = 0;
    // Calculate sum
    // 20 to left, 20 to right
    // int initial_x = 275;
    // int initial_y = 95;
    // int t_height_10 = t_height + 20;
    // int t_width_10 = t_width + 20;

    // temp1 = sqrt(temp1);
    // temp2 = sqrt(temp2);
    double max = 0;
    for (int a = initial_y; a < initial_y + 20; a++)
    {
        for (int b = initial_x; b < initial_x + 20; b++)
        {
            int starting_pos = a * i_width * i_channel + b * i_channel;
            double sum_sim = 0;
            double denominator = 0;
            double temp1 = 0;
            double temp2 = 0;

            for (int i = 0; i < t_height; i++)
            {
                for (int j = 0; j < t_width; j++)
                {
                    for (int k = 0; k < t_channel; k++)
                    {
                        sum_sim += image[starting_pos + i * i_width * i_channel + j * t_channel + k] * template[i * t_width * t_channel + j * t_channel + k];
                        temp1 += pow(image[starting_pos + i * i_width * i_channel + j * t_channel + k], 2);
                        temp2 += pow(template[i * t_width * t_channel + j * t_channel + k], 2);
                    }
                }
            }
            denominator = sqrt(temp1) * sqrt(temp2);
            double cos_sim = sum_sim / denominator;
            if (cos_sim > max)
            {
                max = cos_sim;
                result[0] = b;
                result[1] = a;
            }
            // Calculate cosine similarity
            // printf("%f\n", sum_sim / denominator);
        }
    }

    initial_x = result[0];
    initial_y = result[1];

    // Copy into new template
    for (int i = initial_y; i < initial_y + t_height; i++) {
        for (int j = initial_x; j < initial_x + t_width; j++) {
            int t_location = (i-initial_y) * t_width * t_channel + (j- initial_x) * t_channel;
            int i_location = i * i_width * i_channel + j * i_channel;
            for (int k = 0 ; k < t_channel; k++) {
                template[t_location + k] = image[i_location + k];
            }
        }
    }


    for (int i = initial_x; i < initial_x + t_width; i++)
    {
        int temp = initial_y * i_width * i_channel + i * i_channel;
        image[temp + 0] = 255;
        image[temp + 1] = 255;
        image[temp + 2] = 0;
    }

    for (int i = initial_y; i < initial_y + t_height; i++)
    {
        int temp = i * i_width * i_channel + initial_x * i_channel;
        image[temp + 0] = 255;
        image[temp + 1] = 255;
        image[temp + 2] = 0;
    }

    for (int i = initial_x; i < initial_x + t_width; i++)
    {
        int temp = (initial_y + t_height) * i_width * i_channel + i * i_channel;
        image[temp + 0] = 255;
        image[temp + 1] = 255;
        image[temp + 2] = 0;
    }

    for (int i = initial_y; i < initial_y + t_height; i++)
    {
        int temp = i * i_width * i_channel + (initial_x + t_width) * i_channel;
        image[temp + 0] = 255;
        image[temp + 1] = 255;
        image[temp + 2] = 0;
    }

    return result;
}

int main()
{
    int t_width, t_height, t_channel;
    int i_width, i_height, i_channel;
    char template_path[] = "./template.jpg";
    char write_path[50] = "./result/img";
    int curr_img = 0;
    int *result = malloc(sizeof(int) * 2);

    unsigned char *template_image = stbi_load(template_path, &t_width, &t_height, &t_channel, 0);
    // char test[50]= "test";
    // char test2[50]= "test";
    // itoa(12, test2, 10);
    // strcat(test, test2);
    // printf("%s", test);
    // Size of image
    // printf("%d %d %d\n", t_width, t_height, t_channel);


    char img_path[50] = "./images/img";

    // To string from int
    char temp[3];
    itoa(curr_img, temp, 10);

    char img[50] = "";
    strcat(img, img_path);
    strcat(img, temp);
    strcat(img, ".jpg");

    char dest[50] = "";
    strcat(dest, write_path);
    strcat(dest, temp);
    strcat(dest, ".jpg");

    // Solve the first image
    unsigned char *image = stbi_load(img, &i_width, &i_height, &i_channel, 0);
    // printf("%d %d %d\n", i_width, i_height, i_channel);
    result = find_pixel(template_image, image, t_width, t_height, t_channel, i_width, i_height, i_channel, 275, 95);
    stbi_write_png(dest, i_width, i_height, i_channel, image, i_width * i_channel);
    printf("Finish img %d\n", curr_img);
    curr_img++;

    
    while (curr_img <= 62) {
        char w_temp[3];
        itoa(curr_img, w_temp, 10);

        char w_img[50] = "";
        strcat(w_img, img_path);
        strcat(w_img, w_temp);
        strcat(w_img, ".jpg");

        char w_dest[50] = "";
        strcat(w_dest, write_path);
        strcat(w_dest, w_temp);
        strcat(w_dest, ".jpg");

        image = stbi_load(w_img, &i_width, &i_height, &i_channel, 0);
        result = find_pixel(template_image, image, t_width, t_height, t_channel, i_width, i_height, i_channel, result[0] - 15, result[1] - 15);
        stbi_write_png(w_dest, i_width, i_height, i_channel, image, i_width * i_channel);
        printf("Finish img %d\n", curr_img);
        curr_img++;
    }

    // use the first image for all others
    

    // stbi_write_png(write_path, t_width, t_height, t_channel, template_image, t_width * t_channel);
}