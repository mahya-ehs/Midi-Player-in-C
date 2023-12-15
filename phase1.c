#include <stdio.h>
#include <string.h>
#include "beep.h"
int convert(char str[])         //this function converts the notes into frequnecy
{
    int x=1;
    int nom=str[1]-'0';         
   
    for(int i=0;i<nom;i++)      //note: the frequency of A1=A0*2^1 , A2=A1*2^2 , ...
    {
        x=x*2;
    }
if(str[1]!='s')         //this condition is for notes without #(=s)
{
    if(str[0]=='A')
        return 27.50*x;

    if(str[0]=='B')
        return 30.87*x;

    if(str[0]=='C')
        return 16.35*x;

    if(str[0]=='D')
        return 18.35*x;

    if(str[0]=='E')
        return 20.60*x;

    if(str[0]=='F')
        return 21.83*x;

    if(str[0]=='G')
        return 24.50*x;

}
else
{
    if(str[0]=='A')
        return 29.14*x;

    if(str[0]=='C')
        return 17.32*x;

    if(str[0]=='D')
        return 19.45*x;

    if(str[0]=='F')
        return 23.12*x;

    if(str[0]=='G')
        return 16.35*x;
}
}


int main()
{
    char useless[5], str[5];            //the useless array is defined for the parts of the note that we don't need, like -n
    int i=1, duration,k;
	FILE*inpute;
    inpute=fopen("NOTES.txt","r");
    fscanf(inpute,"%s", str);
    fscanf(inpute,"%s", str);
    int length=strlen(str);
    fscanf(inpute,"%s",useless);
    fscanf(inpute,"%d",&duration);
    beep(convert(str),duration);

    for(k=2;k<=99;k++)
    {
        fscanf(inpute,"%s",str);
        fscanf(inpute,"%s",str);
        fscanf(inpute,"%s",str);
        length=strlen(str);
        fscanf(inpute,"%s",useless);
        fscanf(inpute,"%d",&duration);
        beep(convert(str),duration+200);        
    }
    fclose(inpute);
}

