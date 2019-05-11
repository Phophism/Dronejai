#ifndef PLOTLYMESSENGER_H
#define PLOTLYMESSENGER_H

#include <YunMessenger.h>

class plotly {
    public:
        plotly(char *stream_token_);
        plotly(char *stream_token_, char *timezone_);
        char *stream_token;
        char *timezone;

        // Interfaces for permutation of types: 
        // int, unsigned long, and longs will get casted into floats 
        // plot with 1 argument will set the "x" value to date on the Linino
        // plot with 2 arguments will set the "x" value to the first argument

        void plot( float y );
        void plot( char *y );
        void plot( String y );

        void plot(float x, float y);
        void plot(char *x, float y);
        void plot(String x, float y);

        void plot(float x, char *y);
        void plot(char *x, char *y);
        void plot(String x, char *y);

        void plot(float x, String y);
        void plot(char *x, String y);
        void plot(String x, String y);

    private:
        void preprint_();
        void preprinttimezone_();
        void midprint_();
        void postprint_();
};

#endif



















