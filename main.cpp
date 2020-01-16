#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <math.h>
#include <fstream>

/*include my own config file in order to separate some variables from this file*/
#include "config.h"

sig_atomic_t sigusrsig = 0;
void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("Received SIGUSR1!\n");
        sigusrsig = 1;
    }
}



int main(){
        int device = open(devname, O_RDONLY);
        struct input_event ev;
        /*holds information which finger is currently being tracked*/
        int finger = 0;
        /*holds information about the amount of fingers touching the screen*/
        int nfingers = 0;
        /*placeholder to help switching xmax and ymax*/
        int yrotmax = xmax;
        int xrotmax = ymax;
        /*get info from accelerometer script*/
        double accelscaling = 0.000009806;
        std::string xaccelpathfull = accelpath + "iio:device0/" + xrawdata;
        std::string yaccelpathfull = accelpath + "iio:device0/" + yrawdata;
        std::ifstream xacceldata;
        std::ifstream yacceldata;
        xacceldata.open(xaccelpathfull);
        yacceldata.open(yaccelpathfull);
        if(xacceldata.good() != true){
            xaccelpathfull = accelpath + "iio:device1/" + xrawdata;
            yaccelpathfull = accelpath + "iio:device1/" + yrawdata;
            xacceldata.open(xaccelpathfull);
            yacceldata.open(yaccelpathfull);
        }
        int accelxraw = 0 ;
        int accelyraw = 0 ;
        double accelx = 0 ;
        double accely = 0 ;
        double gthresh = 7.0 ;
        int orientation = 0;
        /*conversion of the angles from radians to degree*/
        double radtoang = 360/6.28318531;
        /*two arrays per finger to track*/
        std::vector<int> finger0x;
        std::vector<int> finger0y;
        std::vector<int> finger1x;
        std::vector<int> finger1y;
        std::vector<int> finger2x;
        std::vector<int> finger2y;
        std::vector<int> finger3x;
        std::vector<int> finger3y;
        std::vector<int> finger4x;
        std::vector<int> finger4y;



        while(1){
            signal(SIGUSR1, my_handler);
            if(sigusrsig == 1){
                sigusrsig = 0;
                xacceldata.clear();
                xacceldata.seekg(0);
                yacceldata.clear();
                yacceldata.seekg(0);
            }
            read(device,&ev, sizeof(ev));
                //if(ev.type == 1 && ev.value == 1){
                //        printf("Key: %i State: %i\n",ev.code,ev.value);
            /*count the number of fingers*/
            if(ev.code == 57 && ev.value > 0){
                nfingers += 1;
            }
            /*determine which finger's coordinates will be incoming*/
            if(ev.code == 47){
                finger = ev.value;
            }
            /*here we need to read the accelerometers and then assign data to the arrays according to the orientation*/
            xacceldata >> accelxraw;
            yacceldata >> accelyraw;
            //printf("accelxraw: %i accelyraw: %i ",accelxraw,accelyraw);
            accelx = accelxraw * accelscaling;
            accely = accelyraw * accelscaling;
            //printf("accelx: %f accely: %f\n",accelx,accely);

            if(accelx < gthresh && accely < -gthresh){
                orientation = 0; /* 0 is normal orientation */
                //printf("Setting orientation to normal\n");
            }else if(accelx > gthresh && accely < gthresh){
                orientation = 1; /* 1 is rotated right*/
                //printf("Setting orientation to right\n");
            }else if (accelx < gthresh && accely > gthresh){
                orientation = 2; /* 2 is upside down*/
                //printf("Setting orientation to upside down\n");
            }else if (accelx < -gthresh && accely < gthresh){
                orientation = 3; /* 3 is rotated left*/
                //printf("Setting orientation to left\n");
            }else{
                //printf("could not find accelerometer data, setting orientation to normal\n");
                orientation = 0;
            }

            /*actually put the data into the arrays*/
            if(orientation == 0){
                //printf("normal orientation\n");
                if(finger == 0){
                        if(ev.code == 53){
                            finger0x.push_back(ev.value);
                        }
                        if(ev.code == 54){
                            finger0y.push_back(ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == 53){
                            finger1x.push_back(ev.value);
                        }
                        if(ev.code == 54){
                            finger1y.push_back(ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == 53){
                            finger2x.push_back(ev.value);
                        }
                        if(ev.code == 54){
                            finger2y.push_back(ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == 53){
                            finger3x.push_back(ev.value);
                        }
                        if(ev.code == 54){
                            finger3y.push_back(ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == 53){
                            finger4x.push_back(ev.value);
                        }
                        if(ev.code == 54){
                            finger4y.push_back(ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if(orientation == 1){
                //printf("rotated right\n");
                xmax = xrotmax;
                ymax = yrotmax;
                if(finger == 0){
                        if(ev.code == 54){
                            finger0x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 53){
                            finger0y.push_back(ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == 54){
                            finger1x.push_back(ymax - ev.value);
                        }
                        if(ev.code == 53){
                            finger1y.push_back(ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == 54){
                            finger2x.push_back(ymax - ev.value);
                        }
                        if(ev.code == 53){
                            finger2y.push_back(ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == 54){
                            finger3x.push_back(ymax - ev.value);
                        }
                        if(ev.code == 53){
                            finger3y.push_back(ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == 54){
                            finger4x.push_back(ymax - ev.value);
                        }
                        if(ev.code == 53){
                            finger4y.push_back(ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if(orientation == 2){
                //printf("upside down\n");
                if(finger == 0){
                        if(ev.code == 53){
                            finger0x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 54){
                            finger0y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == 53){
                            finger1x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 54){
                            finger1y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == 53){
                            finger2x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 54){
                            finger2y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == 53){
                            finger3x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 54){
                            finger3y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == 53){
                            finger4x.push_back(xmax - ev.value);
                        }
                        if(ev.code == 54){
                            finger4y.push_back(ymax - ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }else if (orientation == 3){
                //printf("rotated left\n");
                xmax = xrotmax;
                ymax = yrotmax;
                if(finger == 0){
                        if(ev.code == 54){
                            finger0x.push_back(ev.value);
                        }
                        if(ev.code == 53){
                            finger0y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 1){
                        if(ev.code == 54){
                            finger1x.push_back(ev.value);
                        }
                        if(ev.code == 53){
                            finger1y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 2){
                        if(ev.code == 54){
                            finger2x.push_back(ev.value);
                        }
                        if(ev.code == 53){
                            finger2y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 3){
                        if(ev.code == 54){
                            finger3x.push_back(ev.value);
                        }
                        if(ev.code == 53){
                            finger3y.push_back(ymax - ev.value);
                        }
                    }
                else if(finger == 4){
                        if(ev.code == 54){
                            finger4x.push_back(ev.value);
                        }
                        if(ev.code == 53){
                            finger4y.push_back(ymax - ev.value);
                        }
                }
                else{
                    printf("There was no array planned for more than 5 fingers.\n");
                }
            }
            //printf("array sizes %i,%i,%i,%i,%i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());

            if(ev.code == 57 && ev.value == -1){ /* this code+value means that a finger left the screen and I think that is when the gesture should end, not at the last finger.*/
                /*stuff to calculate per finger*/
                double x0first = finger0x[0];
                double x0last = finger0x[finger0x.size()-1];
                double x0len = ( (x0last - x0first) / xmax );
                double y0first = finger0y[0];
                double y0last = finger0y[finger0y.size()-1];
                double y0len = ( (y0last - y0first) / ymax );
                double finger0directionality = abs(x0len / y0len);
                //printf("directionality: %f\n",fing0directionality);
                double x1first,x1last,x1len,y1first,y1last,y1len,finger1directionality;
                double x2first,x2last,x2len,y2first,y2last,y2len,finger2directionality;
                double x3first,x3last,x3len,y3first,y3last,y3len,finger3directionality;
                double x4first,x4last,x4len,y4first,y4last,y4len,finger4directionality;


                /*only try reading from vectors that are not empty*/
                if(finger1x.size() != 0){
                    x1first = finger1x[0];
                    x1last = finger1x[finger1x.size()-1];
                    x1len = ( (x1last - x1first) / xmax );
                    y1first = finger1y[0];
                    y1last = finger1y[finger1y.size()-1];
                    y1len = ( (y1last - y1first) / ymax );
                    finger1directionality = abs(x1len / y1len);
                }
                if(finger2x.size() != 0){
                    x2first = finger2x[0];
                    x2last = finger2x[finger2x.size()-1];
                    x2len = ( (x2last - x2first) / xmax );
                    y2first = finger2y[0];
                    y2last = finger2y[finger2y.size()-1];
                    y2len = ( (y2last - y2first) / ymax );
                    finger2directionality = abs(x2len / y2len);
                }
                if(finger3x.size() != 0){
                    x3first = finger3x[0];
                    x3last = finger3x[finger3x.size()-1];
                    x3len = ( (x3last - x3first) / xmax );
                    y3first = finger3y[0];
                    y3last = finger3y[finger3y.size()-1];
                    y3len = ( (y3last - y3first) / ymax );
                    finger3directionality = abs(x3len / y3len);
                }
                if(finger4x.size() != 0){
                    x4first = finger4x[0];
                    x4last = finger4x[finger4x.size()-1];
                    x4len = ( (x4last - x4first) / xmax );
                    y4first = finger4y[0];
                    y4last = finger4y[finger4y.size()-1];
                    y4len = ( (y4last - y4first) / ymax );
                    finger4directionality = abs(x4len / y4len);
                }
                //printf("nfingers: %i\n",nfingers);

                    if(nfingers == 1){
                        /*one finger screen edge gestures*/
                        if(y0first >= (ymax - 10) && finger0directionality < 1){
                            printf("swipe from bottom!\n");
                            system(commands[0]);
                        }
                        if(y0first <= 10 && finger0directionality < 1){
                            printf("swipe from top!\n");
                            system(commands[1]);
                        }
                        if(x0first >= (xmax - 10) && finger0directionality > 1){
                            printf("swipe from right!\n");
                            system(commands[2]);
                        }
                        if(x0first <= 10 && finger0directionality > 1){
                            printf("swipe from left!\n");
                            system(commands[3]);
                        }
                    }
                    else if(nfingers == 2){
                            /*calculating values for 2 finger rotations*/
                            double xfirstcom = (x0first + x1first) / 2;
                            double yfirstcom = (y0first + y1first) / 2;
                            double xlastcom  = (x0last + x1last) / 2;
                            double ylastcom = (y0last + y1last) / 2;
                            double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                            //printf("comdist: %f\n",comdist);
                            double vec0xfirst = x0first - xfirstcom;
                            double vec0yfirst = y0first - yfirstcom;
                            double vec1xfirst = x1first - xfirstcom;
                            double vec1yfirst = y1first - yfirstcom;
                            double vec0xlast = x0last - xlastcom;
                            double vec0ylast = y0last - ylastcom;
                            double vec1xlast = x1last - xlastcom;
                            double vec1ylast = y1last - ylastcom;
                            double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                            double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                            int angleavg = round((phi0 + phi1) / 2);
                            int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;
                            /*conditions for 2 finger gestures*/
                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance)){

                            if(finger0directionality < 1 && finger1directionality < 1 && y0last > y0first && y1last > y1first){
                                printf("2 finger swipe down!\n");
                                system(commands[4]);
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && y0last < y0first && y1last < y1first){
                                printf("2 finger swipe up!\n");
                                system(commands[5]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && x0last > x0first && x1last > x1first){
                                printf("2 finger swipe right!\n");
                                system(commands[6]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && x0last < x0first && x1last < x1first){
                                printf("2 finger swipe left!\n");
                                system(commands[7]);
                            }

                        }
                        else if(comdist < comdisttolerance){
                                if(angleavg < 0){
                                    std::string tmpcommand = commands[8] + std::to_string(anglescaled);
				    system(tmpcommand.c_str());
                                }else{
                                    std::string tmpcommand = commands[9] + std::to_string(anglescaled);
                                    system(tmpcommand.c_str());
                                }
                           // printf("I think this is a two finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                        }
                    }
                    else if(nfingers == 3){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first) / 3;
                        double yfirstcom = (y0first + y1first + y2first) / 3;
                        double xlastcom  = (x0last + x1last + x2last) / 3;
                        double ylastcom = (y0last + y1last + y2last) / 3;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2) / 3);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;

                        if(((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance))){
                            if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first){
                                printf("3 finger swipe down!\n");
                                system(commands[10]);
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first){
                                printf("3 finger swipe up!\n");
                                system(commands[11]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && y0last > y0first && y1last > y1first && y2last > y2first){
                                printf("3 finger swipe right!\n");
                                system(commands[12]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && y0last < y0first && y1last < y1first && y2last < y2first){
                                printf("3 finger swipe left!\n");
                                system(commands[13]);
                            }
                        }
                        else if(comdist < comdisttolerance){
                            if(angleavg < 0){
                                    std::string tmpcommand = commands[14] + std::to_string(abs(anglescaled));
                                    system(tmpcommand.c_str());
                                }else{
                                    std::string tmpcommand = commands[15] + std::to_string(anglescaled);
                                    system(tmpcommand.c_str());
                                }
                            //printf("I think this is a three finger rotation with an angle of %i degree scaled to %i\n",angleavg,anglescaled);
                        }
                    }
                    else if(nfingers == 4){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first + x3first) / 4;
                        double yfirstcom = (y0first + y1first + y2first + y3first) / 4;
                        double xlastcom  = (x0last + x1last + x2last + x3last) / 4;
                        double ylastcom = (y0last + y1last + y2last +y3last) / 4;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec3xfirst = x3first - xfirstcom;
                        double vec3yfirst = y3first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double vec3xlast = x3last - xlastcom;
                        double vec3ylast = y3last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        double phi3 = atan2((vec3xfirst*vec3ylast - vec3yfirst*vec3xlast),(vec3xfirst*vec3xlast + vec3yfirst*vec3ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2 + phi3) / 4);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;

                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance && abs(x3len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance && abs(y3len) > swipetolerance)){
                            if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("4 finger swipe down!\n");
                                system(commands[16]);
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("4 finger swipe up!\n");
                                system(commands[17]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("4 finger swipe right!\n");
                                system(commands[18]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("4 finger swipe left!\n");
                                system(commands[19]);
                            }
                        }
                        else if(comdist < comdisttolerance){
                            if(angleavg < 0){
                                std::string tmpcommand = commands[20] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }else{
                                std::string tmpcommand = commands[21] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }
                        }
                    }
                    else if(nfingers == 5){
                        /*calculating values for 3 finger rotations*/
                        double xfirstcom = (x0first + x1first + x2first + x3first + x4first) / 5;
                        double yfirstcom = (y0first + y1first + y2first + y3first + y4first) / 5;
                        double xlastcom  = (x0last + x1last + x2last + x3last + x4last) / 5;
                        double ylastcom  = (y0last + y1last + y2last + y3last + y4last) / 5;
                        double comdist = sqrt((xfirstcom - xlastcom)*(xfirstcom - xlastcom) + (yfirstcom - ylastcom)*(yfirstcom - ylastcom));
                        //printf("comdist: %f\n",comdist);
                        double vec0xfirst = x0first - xfirstcom;
                        double vec0yfirst = y0first - yfirstcom;
                        double vec1xfirst = x1first - xfirstcom;
                        double vec1yfirst = y1first - yfirstcom;
                        double vec2xfirst = x2first - xfirstcom;
                        double vec2yfirst = y2first - yfirstcom;
                        double vec3xfirst = x3first - xfirstcom;
                        double vec3yfirst = y3first - yfirstcom;
                        double vec4xfirst = x4first - xfirstcom;
                        double vec4yfirst = y4first - yfirstcom;
                        double vec0xlast = x0last - xlastcom;
                        double vec0ylast = y0last - ylastcom;
                        double vec1xlast = x1last - xlastcom;
                        double vec1ylast = y1last - ylastcom;
                        double vec2xlast = x2last - xlastcom;
                        double vec2ylast = y2last - ylastcom;
                        double vec3xlast = x3last - xlastcom;
                        double vec3ylast = y3last - ylastcom;
                        double vec4xlast = x4last - xlastcom;
                        double vec4ylast = y4last - ylastcom;
                        double phi0 = atan2((vec0xfirst*vec0ylast - vec0yfirst*vec0xlast),(vec0xfirst*vec0xlast + vec0yfirst*vec0ylast))*radtoang;
                        double phi1 = atan2((vec1xfirst*vec1ylast - vec1yfirst*vec1xlast),(vec1xfirst*vec1xlast + vec1yfirst*vec1ylast))*radtoang;
                        double phi2 = atan2((vec2xfirst*vec2ylast - vec2yfirst*vec2xlast),(vec2xfirst*vec2xlast + vec2yfirst*vec2ylast))*radtoang;
                        double phi3 = atan2((vec3xfirst*vec3ylast - vec3yfirst*vec3xlast),(vec3xfirst*vec3xlast + vec3yfirst*vec3ylast))*radtoang;
                        double phi4 = atan2((vec4xfirst*vec4ylast - vec4yfirst*vec4xlast),(vec4xfirst*vec4xlast + vec4yfirst*vec4ylast))*radtoang;
                        int angleavg = round((phi0 + phi1 + phi2 + phi3 + phi4) / 5);
                        int anglescaled = (angleavg / anglescaling) - (angleavg / anglescaling) % anglestepping ;

                        if((abs(x0len) > swipetolerance && abs(x1len) > swipetolerance && abs(x2len) > swipetolerance && abs(x3len) > swipetolerance && abs(x4len) > swipetolerance) || (abs(y0len) > swipetolerance && abs(y1len) > swipetolerance && abs(y2len) > swipetolerance && abs(y3len) > swipetolerance && abs(y4len) > swipetolerance)){
                            if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("5 finger swipe down!\n");
                                system(commands[22]);
                            }
                            else if(finger0directionality < 1 && finger1directionality < 1 && finger2directionality < 1 && finger3directionality < 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("5 finger swipe up!\n");
                                system(commands[23]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && y0last > y0first && y1last > y1first && y2last > y2first && y3last > y3first){
                                printf("5 finger swipe right!\n");
                                system(commands[24]);
                            }
                            else if(finger0directionality > 1 && finger1directionality > 1 && finger2directionality > 1 && finger3directionality > 1 && y0last < y0first && y1last < y1first && y2last < y2first && y3last < y3first){
                                printf("5 finger swipe left!\n");
                                system(commands[25]);
                            }
                        }
                        else if(comdist < comdisttolerance){
                            if(angleavg < 0){
                                std::string tmpcommand = commands[26] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }else{
                                std::string tmpcommand = commands[27] + std::to_string(anglescaled);
                                system(tmpcommand.c_str());
                            }
                        }
                    }

                //printf("arrays before clear: %i %i %i %i %i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());

                /*empty the vectors*/
                finger0x.clear();
                finger0y.clear();
                finger1x.clear();
                finger1y.clear();
                finger2x.clear();
                finger2y.clear();
                finger3x.clear();
                finger3y.clear();
                finger4x.clear();
                finger4y.clear();

                x0first = 0;
                x0last = 0;
                x0len = 0;
                y0first = 0;
                y0last = 0;
                y0len = 0;
                finger0directionality = 0;
                x1first = 0;
                x1last = 0;
                x1len = 0;
                y1first = 0;
                y1last = 0;
                y1len = 0;
                finger1directionality = 0;
                x2first = 0;
                x2last = 0;
                x2len = 0;
                y2first = 0;
                y2last = 0;
                y2len = 0;
                finger2directionality = 0;
                x3first = 0;
                x3last = 0;
                x3len = 0;
                y3first = 0;
                y3last = 0;
                y3len = 0;
                finger3directionality = 0;
                x4first = 0;
                x4last = 0;
                x4len = 0;
                y4first = 0;
                y4last = 0;
                y4len = 0;
                finger4directionality = 0;

                //printf("arrays after clear: %i %i %i %i %i\n",finger0x.size(),finger1x.size(),finger2x.size(),finger3x.size(),finger4x.size());

                nfingers = 0;
            }
        }
}
