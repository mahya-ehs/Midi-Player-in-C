//purpose : playing midi files

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "beep.h"

int isNoteOn = 0;
int ChangeEndian();     
int shift();
unsigned long deltatime_calculation(FILE*midifile);
float note_frequency(FILE* midifile, char note);
int tempo_calculation(FILE*midifile);
int read_events(unsigned char byte, FILE*midifile);


int main()
{
    unsigned int delta_time = 0, time, duration;
    float frequency;
    unsigned char velocity, note;

    FILE*midifile;          
    midifile = fopen("5.mid","rb");

    char chunk[4], trackchunk[4];
    unsigned char format[2], division[2], tracks[2];
    int length1, length2, endOfFile = 0;
    chunk[4] = '\0';

    fread(chunk, 4, 1, midifile); 
    puts(chunk);  
    fread(&length1, 4, 1, midifile);
    fread(format, 2, 1, midifile);
    fread(tracks, 2, 1, midifile);
    fread(division, 2, 1, midifile);
    fread(trackchunk, 4, 1, midifile);
    fread(&length2, 4, 1, midifile);

    int division_dec = shift(division);
    int num_of_bytes = ChangeEndian(length2);

    printf("length : %d\n",ChangeEndian(length1));      //printing header chunk and header track info
    printf("format %d\n%d tracks\ndivision : %d ticks / 1/4 note\n", shift(format), shift(tracks), shift(division));
    printf("track chunk : %s\nlength : %d\n", trackchunk, ChangeEndian(length2));

    printf("\n\nSTART!\n\n");

    while(1)
    {
        if(endOfFile)
            break;
        while(1)  
        {
            unsigned char byte;
            unsigned char tempo[10000],len;
            delta_time = deltatime_calculation(midifile);
            printf("delta-time : %X\n", delta_time);
            fread(&byte, 1, 1, midifile);
            printf("byte : %X\n", byte);
            unsigned int tempodecimal;
            int result = read_events(byte, midifile);

            if(result == -1)            //EOF
            {
                endOfFile = 1;
                break;
            }         
                
            if(result == 1)     //tempo
            {
                fread(&len, 1, 1, midifile);              
                fread(tempo, 3, 1, midifile);
                tempodecimal = tempo[2] | tempo[1] << 8 | tempo[0] << 16; 
                printf("tempo is : %d\n",tempodecimal);
            }
            if(result == 2)             //note on
            {
                time = delta_time + time;
                fread(&note, 1, 1, midifile);
                frequency = note_frequency(midifile,note);
                fread(&velocity, 1, 1, midifile);
                printf("frequency is : %f\n",frequency);
            }
            if(result == 3)         //note off
            {
                time = ((1000 * division_dec) / tempodecimal) * delta_time;
                beep(frequency, time + 700);
                unsigned char trash[10000];
                fread(trash, 2, 1, midifile);                                                        

            }
            else if(result == 0)        //other conditions
                break;
        }}
    
        
    printf("The End!\n");
    fclose(midifile);
}



int read_events(unsigned char byte, FILE* midifile)     //this function shows the events(events can be anything like lyrics, text, note off/on)
{
    unsigned char event,len;
    unsigned char trash[10000];
    if(byte == 0xFF)
    {
        fread(&event, 1, 1, midifile);
        printf("event : %X\n",event);
        if(event == 0x2F)
            return -1;
            
        else if(event == 0x51)
            return 1;
            
        else if(event == 0x00)
        {   
            fread(&len, 1, 1, midifile);                                     
            fread(&trash, 2, 1, midifile);
            printf("Event : Sequence Number\n");
            return 0;

        }
            
        else if(event == 0x01)
        {
            fread(&len, 1, 1, midifile);
            fread(&trash, len, 1, midifile);
            printf("Event : Text Event\n");
            return 0;
        }
            
        else if(event == 0x02)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len,1,midifile);
            printf("Event : Copyright Notice\n");
            return 0;

        }

        else if(event == 0x03)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len ,1 ,midifile);
            printf("Event : Sequence/Track Name\n");
            return 0;
        }                
        else if(event == 0x04)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len ,1 ,midifile);
            printf("Event : Instrument Name\n");
            return 0;
        }                

        else if(event == 0x05)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len,1,midifile);
            printf("Event : Lyric\n");
            return 0;
        }
        else if(event == 0x06)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len ,1 ,midifile);
            printf("Event : Marker\n");
            return 0;
        }

        else if(event == 0x07)
        {
            fread(&len,1 ,1 ,midifile);
            fread(&trash,len ,1 ,midifile);
            printf("Event : Cue Point\n");
            return 0;
        }
        else if(event == 0x20)
        {
            fread(&len,1 ,1 ,midifile);                                   
            fread(&trash,1 ,1 ,midifile);
            printf("Event : MIDI Channel Prefix\n");
            return 0;
        }
        else if(event == 0x54)
        {
            fread(&len, 1, 1, midifile);                                   
            fread(&trash, 5, 1, midifile);
            printf("Event : SMPTE Offset\n");
            return 0;
        }
        else if(event == 0x58)
        {
            fread(&len, 1, 1, midifile);                                  
            fread(&trash, 4, 1, midifile);
            printf("Event : Time Signature\n");
            return 0;
        }

        else if(event == 0x59)
        {
            fread(&len, 1, 1, midifile);                              
            fread(&trash,2,1,midifile);
            printf("Event : Key Signature\n");
            return 0;
        }
                
        else if(event == 0x7F)
        {
            fread(&len, 1, 1, midifile);
            fread(&trash,len,1,midifile);
            printf("Event : Sequencer Specific Meta Event\n");
            return 0;
        }
            
    }
    
    if(byte == 0xF0)
    {
        len = deltatime_calculation(midifile);
        fread(&trash, len, 1, midifile);
        printf("Event : Normal SysEx Events\n");
        return 0;
    }
    if(byte == 0xF7)
    {
        len = deltatime_calculation(midifile);
        fread(&trash, len, 1, midifile);
        printf("Event : Divided SysEx Events\n");
        return 0;
    }
    if(byte >= 0xc0 && byte < 0xe0)
    {
        fread(&trash, 1, 1, midifile);
        return 0;
    }
    if(byte >= 0x90 && byte <= 0x9F)                        
    {
        printf("Event : note on\n");
        isNoteOn = 1;
        return 2;
    }
    if(byte >= 0x80 && byte < 0x90 )                                                
    {
        printf("Event : note off\n");
        return 3;
    }
    if(byte >= 0xC0 && byte < 0xE0)
    {
        fread(&trash, 1, 1, midifile);
        return 0;
    }
    
    else
    {
        fread(&trash, 2, 1, midifile);
        return 0;
    }
}


float note_frequency(FILE* midifile,char note)      //this function converts notes to frequency
{
    int note_in_decimal = (int)note;
    float frequency;
    int octave = note_in_decimal % 12;
    switch(octave)
    {
        case 0:
            frequency = pow(2, (note_in_decimal / 12)) * 16.35;
            return frequency; 
            break;

        case 1:
            frequency = pow(2, (note_in_decimal / 12)) * 17.32;
            return frequency;
            break;

        case 2:
            frequency = pow(2, (note_in_decimal / 12)) * 18.35;
            return frequency;
            break;

        case 3:
            frequency = pow(2, (note_in_decimal / 12)) * 19.45;
            return frequency;
            break;

        case 4:
            frequency = pow(2, (note_in_decimal / 12 )) * 20.60;
            return frequency;
            break;

        case 5:
            frequency = pow(2, (note_in_decimal / 12)) * 21.83;
            return frequency;
            break;

        case 6:
            frequency = pow(2, (note_in_decimal / 12)) * 23.12;
            return frequency;
            break;

        case 7:
            frequency = pow(2, (note_in_decimal / 12)) * 24.50;
            return frequency;
            break;

        case 8:
            frequency = pow(2, (note_in_decimal / 12)) * 16.35;
            return frequency;
            break;

        case 9:
            frequency = pow(2, (note_in_decimal / 12)) * 27.50;
            return frequency;
            break;

        case 10:
            frequency = pow(2, (note_in_decimal / 12)) * 29.14;
            return frequency;
            break;

        case 11:
            frequency = pow(2, (note_in_decimal / 12)) * 30.87;
            return frequency;
            break;
    }

}



int tempo_calculation(FILE *midifile)
{
    unsigned char tempo[4], dummy;
    fread(&dummy, 1, 1, midifile);
    fread(tempo, 3, 1, midifile);
    return tempo[0] << 16 | tempo[1] << 8 | tempo[2];
}

int ChangeEndian(int num) 
{
    return (((num>>24) & 0x000000ff) | ((num<<8) & 0x00ff0000) | ((num>>8) & 0x0000ff00) | ((num<<24) & 0xff000000));
}          
  
int shift(unsigned char x[2])           
{
    return x[1] | x[0] << 8;
}

unsigned long deltatime_calculation(FILE*midifile)      
{
    unsigned int value;
    unsigned char x;
    value = fgetc(midifile);
    if(value & 0x80)                
    {
        value &= 0x7F;
        do
        {
            x = fgetc(midifile);
            value = (value << 7) + (x & 0x80);
        }while(x & 0x80);
    }

    return value;
}




