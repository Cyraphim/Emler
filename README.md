# Emler

Emler is a machine learning program that I made after watching the CS50 lecture on machine learning written in C just because. The code isnt the best written I found a lot of repeated code and I limited it in places where I didnt really need to, but I am kinda afraid to tamper with it since it already works. Alot of it is derivative of code from previous CS50 projects. 

Once you start it up, it prompts you to input the name of the image you would like to add. It only accepts bitmaps for the moment.
It then parses the image and creates 64 "coordinates" where 1 represents Black and 0 represents White. It then plots them on a 64 dimensional graph and checks for the nearest image on there. If it doesnt find one, it asks you to name it otherwise it asks you if the image you gave it is the same as the image it found in its data. If not, you will have to name it. 
