//Data Structure of the image. 
typedef struct imagePlot {
        string image;                           // Name of the image
        struct imagePlot *next;                 // Pointer to the next Data Structure
        int coordinates[64];                    // 64 coordinates for 64 pixels each being either 0 or 1
}imagePlot;

// Asks user for filename
string askfile();       

// Attaches data structure into a linked list
int makefile(string name);

// Locates the file asked for or returns the closest image
imagePlot locfile (imagePlot img);

// Square root formula for calculating distance
double dist (imagePlot curimg, imagePlot myimg);

// Deletes everything from linked list
bool unload (void);

// Clears the entire scree using ASCII escape sequences
void clear(void);

// Fun
void making(void);