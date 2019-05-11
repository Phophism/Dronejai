#include "PlotlyYun.h"
#include "YunMessenger.h"

plotly::plotly(char *stream_token_){
    stream_token = stream_token_;
}

plotly::plotly(char *stream_token_, char *timezone_){
    stream_token = stream_token_;
    timezone = timezone_;
}

void plotly::plot( float y ){
    preprinttimezone_();
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}

void plotly::plot( char *y ){
    preprinttimezone_();    
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}

void plotly::plot( String y ){
    preprinttimezone_();
    midprint_();
    Console.print("\"");
    Console.print(y);
    Console.print("\"");    
    postprint_();
}

void plotly::plot(float x, float y){
    /*
     * Send a plotly-formatted JSON object
     * to the plotly subscriber on the Linino
     *
     * Example object:
     * {"x": 1, "y": 10}
    */

    //Messenger::send("plotly", "{\"x\": 1, \"y\": 10}");

    preprint_(); 
    Console.print(x);
    midprint_();
    Console.print(y);
    postprint_();
}

void plotly::plot(char *x, float y){
    preprint_();
    Console.print("\"");
    Console.print(x);
    Console.print("\"");    
    midprint_();
    Console.print(y);
    postprint_();
}
void plotly::plot(String x, float y){
    preprint_();
    Console.print(x);
    midprint_();
    Console.print(y);
    postprint_();
}

void plotly::plot(float x, char *y){
    preprint_();
    Console.print(x);
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}
void plotly::plot(char *x, char *y){
    preprint_();
    Console.print("\"");
    Console.print(x);
    Console.print("\"");    
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}
void plotly::plot(String x, char *y){
    preprint_();
    Console.print("\"");    
    Console.print(x);
    Console.print("\"");    
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}

void plotly::plot(float x, String y){
    preprint_();
    Console.print(x);
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}
void plotly::plot(char *x, String y){
    preprint_();
    Console.print("\"");    
    Console.print(x);
    Console.print("\"");    
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}
void plotly::plot(String x, String y){
    preprint_();
    Console.print("\"");    
    Console.print(x);
    Console.print("\"");    
    midprint_();
    Console.print("\"");    
    Console.print(y);
    Console.print("\"");    
    postprint_();
}


void plotly::preprint_(){
    Messenger::open("plotly");
    Console.print("{\"x\": ");
}

void plotly::preprinttimezone_(){
    Messenger::open("plotly");
    Console.print("{\"timezone\": \"");
    Console.print(timezone);
    Console.print("\"");
}

void plotly::midprint_(){
    Console.print(", \"y\": ");
}

void plotly::postprint_(){
    Console.print(", \"token\": \"");
    Console.print(stream_token);
    Console.print("\"}");
    Messenger::close();    
}

