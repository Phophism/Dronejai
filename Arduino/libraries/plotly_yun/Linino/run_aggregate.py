#!/usr/bin/python
# -*- coding: utf-8 -*-

from YunMessenger import Console
import plotly
import json
import traceback
import datetime
from pytz import timezone
import sys

# Initialize a YunMessenger console object
try:
    c = Console.Console(log_filename="/root/Plotly.log")
except IOError as e:
    print("ERROR: The log file directory that you specified does not exist")
    raise e

# Load up plotly credentials
config_filename = './config.json'
try:
    with open(config_filename) as config_file:
        plotly_user_config = json.load(config_file)
except IOError as e:
    helpful_msg = "Please place a configuration file at {filename}"\
        " with your plotly credentials and try again.\n"\
        "Questions? Need some help? Support@plot.ly"\
        .format(filename=config_filename)
    print(helpful_msg)
    c.logger.error(helpful_msg)
    raise e
except ValueError as e:
    helpful_msg = "The JSON config file at {filename} could not be decoded, "\
        "are you sure that it is in the right format?\n"\
        "Questions? Support@plot.ly".format(filename=config_filename)
    print(helpful_msg)
    c.logger.error(helpful_msg)
    raise e

# Extract the credentials from the config file
username = plotly_user_config['plotly_username']
api_key = plotly_user_config['plotly_api_key']
stream_tokens = plotly_user_config['plotly_streaming_tokens']
if 'show_this_many_points' in plotly_user_config:
    maxpoints = plotly_user_config['show_this_many_points']
else:
    maxpoints = 500

# Make the initializing plotly request
# Pass all stream tokens from the config file into this plot
p = plotly.plotly(username, api_key)
p.ioff()
res = p.plot(
    [
        {'x': [], 'y': [], 'stream': {'token': token, 'maxpoints': maxpoints}}
        for token in stream_tokens
    ], filename='Stream from Yun', fileopt='overwrite')

if 'stream-status' in res and 'All Streams Go!' in res['stream-status']:
    status = "Plot initialized at: {url}\nwith token(s): {tokens}"\
        .format(url=res['url'], tokens=', '.join(stream_tokens))
    print(status)
    c.logger.info(status)
else:
    status = "Error initializing plot: {msg}".format(msg=res['error'])
    print(status)
    c.logger.error(status)
    sys.exit(1)


class PlotlyHandler:
    ''' Create a message handler for streams
    that writes to streams that are indexed by token
    '''
    def __init__(self, stream_tokens):
        ''' Store the stream tokens in an object
         so that the message handler can access them
        '''
        self.streams = {}
        self.data = {}
        self.last_write = {} 
        for token in stream_tokens:
            self.streams[token] = plotly.stream(token)
            self.data[token] = [] # {'avg': 0, 'max': 0, 'min': 0, 'n': 0}
            self.last_write[token] = datetime.datetime.now()

    def handlePlotlyMessages(self, msg):
        ''' Write the msg data to the associated plotly stream
        msg is a json string from the Arduino and is of the form:
            {"x": 1, "y": 1, "token": "ibagefhdci"}
            or
            {"y": 1, "token": "ibagefhdci", "timezone": "America/Montreal"}
        If it's the latter form, we'll compute the time and assign that as x
        '''
        data = json.loads(msg)
        print 'data! ', data
        if data['token'] in self.streams:
            token = data['token']
            del data['token']

            # store the data
            #print 'appending'
            self.data[token].append(float(data['y']))

            if ((datetime.datetime.now() - self.last_write[token]) >
               datetime.timedelta(milliseconds=50)):
                # print 'time to write'            
                # then it's time to aggregate, and write

                if 'x' not in data and 'timezone' in data:
                    try:
                        tz = timezone(data['timezone'])
                    except:
                        c.logger.warning("Unrecognized timezone: {timezone}\n"
                                         "Defaulting to America/Montreal"
                                         .format(timezone=data['timezone']))
                        c.logger.debug(traceback.format_exc())
                        tz = timezone("America/Montreal")
                    del data['timezone']
                    data['x'] = tz.localize(datetime.datetime.now())\
                                  .strftime('%Y-%m-%d %H:%M:%S.%f')
                # print 'aggregating: ', self.data
                data['y'] = sum(self.data[token]) / len(self.data[token])
                c.logger.info("Aggregated {N} points"
                              .format(N=len(self.data[token])))
                self.data[token] = []

                self.streams[token].write(data)
                self.last_write[token] = datetime.datetime.now()


        else:
            c.logger.warning("Recieved the token: {token} "
                             "which has no associated stream.\n"
                             "Add the token {token} to your config.json"
                             " file to register this stream."
                             .format(token=data['token']))

plotlyHandler = PlotlyHandler(stream_tokens)

# Assign a "subscriber" to `plotly` messages that are transmitted from plotly
c.onMessage['plotly'] = plotlyHandler.handlePlotlyMessages

# Start listening for messages (data) from the Arduino!
c.run()
