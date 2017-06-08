#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "bmp.h"
#include "Emler.h"

imagePlot *head;

int main(){
    bool done = false;
    while (!done) {
        clear();
        
        string filename = askfile();
        
        int err = makefile(filename);
        
        if (err == 1 || err == 2 || err == 3 || err == 4) {
            printf("There was an error, ");
        }
        
        printf("Would you like to insert another image? (Y/N) \n");
        string answer = get_string();
        
        if (answer[0] == 'n' || answer[0] == 'N') {
            done = true;
            unload();
        }
    }
   return 0;
}

string askfile () {
    printf("Please type in the name of the image\n");
    string fname = get_string();
    return fname;
}

int makefile(string name) {
    
    // open input file 
    FILE *inptr = fopen(name, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", name);
        return 2;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    if (bi.biWidth !=bi.biHeight || bi.biWidth != 8 || bi.biHeight != 8) {
        fclose(inptr);
        fprintf(stderr, "Please input a file of 8x8 only.\n");
        return 0;
    }
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    imagePlot *newimage = malloc(sizeof(imagePlot));
    newimage->next = NULL;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (triple.rgbtRed == 0x00 && triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00) {
                    newimage->coordinates[i*bi.biWidth + j] = 1;
            } else if (triple.rgbtRed == 0xff && triple.rgbtBlue == 0xff && triple.rgbtGreen == 0xff) {
                    newimage->coordinates[i*bi.biWidth + j] = 0;
            }
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }
        
    imagePlot findimage = locfile(*newimage);

    if (findimage.image != NULL) {
        printf("\n\nIs the content of the image : %s ? (Y/N)\n", findimage.image);
        string text = get_string();
        if (text[0] == 'N' || text[0] == 'n') {
            printf("\n\nI dont know what that is. Please tell me!\n");
            string ntext = get_string();
            newimage->image = ntext;    
        } else {
            newimage->image = findimage.image;
        }
    } else {
        printf("\n\nI dont know what that is. Please tell me!\n");
        string text = get_string();
        newimage->image = text;
    }
    
    if (head != NULL) {
        if (head->next != NULL) {
            newimage->next = head->next;
            head->next = newimage;
        } else {
            imagePlot *curimg = head;
            while (curimg->next != NULL) {
                curimg = curimg->next;
            }
        curimg->next = newimage;
        }
    } else {
        head = newimage;
    }

    // close infile
    fclose(inptr);
    
    making();
    
    // success
    return 0;
}

imagePlot locfile(imagePlot img) {
    imagePlot *curnode = head;
    imagePlot *finalnode = &img;
    double newdist = 999;
    
    while (curnode != NULL) {
         
        int count = 0;
        
        for(int i = 0; i < 64; i++) {
            if (curnode->coordinates[i] == img.coordinates[i]) {
                count++;
            }
            if (curnode->coordinates[i] > 1) {
                return img;
            }
        }
        if (count == 64) {
            return *curnode;
        }
        
        double  distance = dist(*curnode, img);
        
        if (distance < newdist) { 
            newdist = distance;
            finalnode = curnode;
        }
        if (curnode->next != NULL) {
            curnode = curnode->next;
        }   else {
            break;
        }
    }
    
    return *finalnode;
}

double dist (imagePlot curimg, imagePlot myimg) {
    double sum = 0.0;
    for (int i = 0; i < 64; i++) {
        int temp = (curimg.coordinates[i] - myimg.coordinates[i])*(curimg.coordinates[i] - myimg.coordinates[i]);
        sum += temp;
    }
    
    return sqrt(sum);
}

bool unload(void)
{
    imagePlot* cursor = head;
    while (cursor != NULL)
    {
        imagePlot* temp = cursor;
        cursor = cursor->next;
        free(temp);
    }
    
    return true;
}

void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void making(void) {
    printf("Saving");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".\n");
    sleep(1);
    clear();
}