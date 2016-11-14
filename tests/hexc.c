#include <stdio.h>  
#include <string.h>  
  
int main()  
{  
    char hex[200];  
    char s[100];  
    strcpy(hex,"1adfkk");  
    int L=strlen(hex), i, v;  
    for (i=0;i<L/2;i++)   
    {  
        sscanf(hex+i*2,"%2X",&v);  
        s[i]=(char)(v&0xFFu);  
    }  
  
    s[i]=0;  
    printf("%s\n",s);  
          
    L=strlen(s);  
    for (i=0;i<L;i++)   
    sprintf(hex+2*i,"%02x",(unsigned char)s[i]);  
  
    hex[2*i+2]=0;  
    printf("%s\n",hex);  
}  
