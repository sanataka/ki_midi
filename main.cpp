#include "mbed.h"

Serial midi(D1,D0); //midi in pin D0
DigitalOut out1(D8); //ch1 sound output
DigitalOut outled1(D9);//sound status1
DigitalOut out2(D10); //ch2 sound output
DigitalOut outled2(D11);//sound status2


AnalogIn vr1(A1);
AnalogIn vr2(A0);

int d1, d2, d3,
    f1_1,f1_2,f1_3,f1_4,f1_5,f1_6,
    f2_1,f2_2,f2_3,f2_4,f2_5,f2_6,
    busy1_1,busy1_2,busy1_3,
    busy2_1,busy2_2,busy2_3,
    hold1_1,hold1_2,hold1_3,
    hold2_1,hold2_2,hold2_3,
    poly_n1,poly_n2;

int min_on = 5;
int pw1,pw2;

int ftime[128] = {122312,115447,108968,102852,97079,91631,86488,81634,77052,72727,68645,64793,
                61156, 57724, 54484, 51426,48540,45815,43244,40817,38526,36364,34323,32396,
                30578, 28862, 27242, 25713,24270,22908,21622,20408,19263,18182,17161,16198,
                15289, 14431, 13621, 12856,12135,11454,10811,10204, 9631, 9091, 8581, 8099,
                 7645,  7215,  6810,  6428, 6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050,
                 3822,  3608,  3405,  3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025,
                 1911,  1804,  1703,  1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012,
                  956,   902,   851,   804,  758,  716,  676,  638,  602,  568,  536,  506,
                  478,   451,   425,   402,  379,  358,  338,  319,  301,  284,  268,  253,
                  239,   225,   213,   201,  190,  179,  169,  159,  150,  142,  134,  127,
                  119,   113,   106,   100,   95,   89,   84,   80};
             
Ticker ch1_1;
Ticker ch1_2;
Ticker ch1_3;
Ticker ch2_1;
Ticker ch2_2;
Ticker ch2_3;
Timeout width1;
Timeout width2;
            
            
void ch1_l(){width1.detach();out1 = 0;}
void ch2_l(){width2.detach();out2 = 0;}
void ch1_h(){width1.detach();pw1 = (vr1 * 255) + min_on;width1.attach_us(&ch1_l,pw1);out1 = 1;}
void ch2_h(){width2.detach();pw2 = (vr2 * 255) + min_on;width2.attach_us(&ch2_l,pw2);out2 = 1;}

int main()
{
    reset:
    ch1_1.detach();busy1_1 = 0;hold1_1 = 255;
    ch1_2.detach();busy1_2 = 0;hold1_2 = 255;
    ch1_3.detach();busy1_3 = 0;hold1_3 = 255;
    ch2_1.detach();busy2_1 = 0;hold2_1 = 255;
    ch2_2.detach();busy2_2 = 0;hold2_2 = 255;
    ch2_3.detach();busy2_3 = 0;hold2_3 = 255;               
    midi.baud(31250);
    midi.format(8,Serial::None,1);
    while(1){
//------------------------------------------------------------------------------
            reget:
            d1 = midi.getc();
            if(d1 >= 0x80 && d1 <= 0x9f){
                d2 = midi.getc();
                d3 = midi.getc();
            }else if(d1 >= 0xb0 && d1 <= 0xbf){
                d2 = midi.getc();
                if(d2 == 0x78 || d2 == 0x79 || d2 == 0x7b){ //reset
                    goto reset;
                    }
            }else{
                goto reget;
                }
            
//------------------------------------------------------------------------------
            switch(d1){
                
            case 0x90:
                if(d3 > 0 && busy1_1 == 0){
                                        f1_1 = ftime[d2];
                                        busy1_1 = 1;
                                        hold1_1 = d2;
                                        ch1_1.attach_us(&ch1_h,f1_1);
                    }else if(d3 > 0 && busy1_2 == 0){
                                        f1_2 = ftime[d2];
                                        busy1_2 = 1;
                                        hold1_2 = d2;
                                        ch1_2.attach_us(&ch1_h,f1_2);
                    }else if(d3 > 0 && busy1_3 == 0){
                                        f1_3 = ftime[d2];
                                        busy1_3 = 1;
                                        hold1_3 = d2;
                                        ch1_3.attach_us(&ch1_h,f1_3);
                                    
//------------------------------------------------------------------------------------------                                        
                    }else if(d3 == 0 && d2 == hold1_1){
                                        ch1_1.detach();
                                        busy1_1 = 0;
                                        hold1_1 = 255;
                    }else if(d3 == 0 && d2 == hold1_2){
                                        ch1_2.detach();
                                        busy1_2 = 0;
                                        hold1_2 = 255;
                    }else if(d3 == 0 && d2 == hold1_3){
                                        ch1_3.detach();
                                        busy1_3 = 0;
                                        hold1_3 = 255;
                }
                break;
                
            

            case 0x91:
                if(d3 > 0 && busy2_1 == 0){
                                        f2_1 = ftime[d2];
                                        busy2_1 = 1;
                                        hold2_1 = d2;
                                        ch2_1.attach_us(&ch2_h,f2_1);
                    }else if(d3 > 0 && busy2_2 == 0){
                                        f2_2 = ftime[d2];
                                        busy2_2 = 1;
                                        hold2_2 = d2;
                                        ch2_2.attach_us(&ch2_h,f2_2);
                    }else if(d3 > 0 && busy2_3 == 0){
                                        f2_3 = ftime[d2];
                                        busy2_3 = 1;
                                        hold2_3 = d2;
                                        ch2_3.attach_us(&ch2_h,f2_3);
                                    
//------------------------------------------------------------------------------------------                                        
                    }else if(d3 == 0 && d2 == hold2_1){
                                        ch2_1.detach();
                                        busy2_1 = 0;
                                        hold2_1 = 255;
                    }else if(d3 == 0 && d2 == hold2_2){
                                        ch2_2.detach();
                                        busy2_2 = 0;
                                        hold2_2 = 255;
                    }else if(d3 == 0 && d2 == hold2_3){
                                        ch2_3.detach();
                                        busy2_3 = 0;
                                        hold2_3 = 255;
                }
                break;
                
            case 0x80:
             if(d2 == hold1_1){
                            ch1_1.detach();
                            busy1_1 = 0;
                            hold1_1 = 255;
                }else if(d2 == hold1_2){
                            ch1_2.detach();
                            busy1_2 = 0;
                            hold1_2 = 255;
                }else if(d2 == hold1_3){
                            ch1_3.detach();
                            busy1_3 = 0;
                            hold1_3 = 255;
                }
            break;
            
            case 0x81:
             if(d2 == hold2_1){
                            ch2_1.detach();
                            busy2_1 = 0;
                            hold2_1 = 255;
                }else if(d2 == hold2_2){
                            ch2_2.detach();
                            busy2_2 = 0;
                            hold2_2 = 255;
                }else if(d2 == hold2_3){
                            ch2_3.detach();
                            busy2_3 = 0;
                            hold2_3 = 255;
                }
            break;
                
            
            default:
                break;
            }
            
//------------------------------------------------------------------------------
            poly_n1 = busy1_1 + busy1_2 + busy1_3;
            if(poly_n1 == 0){outled1 = 0;}else{outled1 = 1;}

            poly_n2 = busy2_1 + busy2_2 + busy2_3;
            if(poly_n2 == 0){outled2 = 0;}else{outled2 = 1;}
//------------------------------------------------------------------------------

    }
}
