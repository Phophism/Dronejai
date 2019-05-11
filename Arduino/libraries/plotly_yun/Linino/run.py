#!/usr/bin/python
# -*- coding: utf-8 -*-

from YunMessenger import Console
import plotly.plotly as py
import json
import traceback
import datetime
import time
from pytz import timezone
import sys
import os

# Initialize a YunMessenger console object
try:
    c = Console.Console(log_filename="/root/Plotly.log")
except IOError as e:
    print("ERROR: The log file directory that you specified does not exist")
    raise e

# Load up plotly credentials
config_filename = '/root/config.json'
try:
    with open(config_filename) as config_file:
        config_string = config_file.read()
        # Unicode song-and-dance to convert curly double quotes to straight single quotes
        config_unicode = unicode(config_string, 'utf-8')
        punctuation = { 0x2018:0x27, 0x2019:0x27, 0x201C:0x22, 0x201D:0x22 }
        normalized_config_string = config_unicode.translate(punctuation).encode('ascii', 'ignore')
        plotly_user_config = json.loads(normalized_config_string)
except IOError as e:
    helpful_msg = "Please place a configuration file at {filename}"\
        " with your plotly credentials and try again.\n"\
        "Questions? Need some help? Support@plot.ly".format(filename=config_filename)
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

# Extract the credentials and settings from the config file
username = plotly_user_config['plotly_username']
api_key = plotly_user_config['plotly_api_key']
stream_tokens = plotly_user_config['plotly_streaming_tokens']
file_option = plotly_user_config['file_option']

if 'show_this_many_points' in plotly_user_config:
    maxpoints = plotly_user_config['show_this_many_points']
else:
    maxpoints = 500

# Make the initializing plotly request
# Pass all stream tokens from the config file into this plot
py.sign_in(username, api_key)

url = py.plot(
    [
        {'x': [], 'y': [], 'stream': {'token': token, 'maxpoints': maxpoints}}
        for token in stream_tokens
    ],
    filename='Stream from Yun',
    auto_open=False,
    fileopt=file_option)


status = "Plot initialized at: {url}\nwith token(s): {tokens}"\
         .format(url=url, tokens=', '.join(stream_tokens))
print(status)
c.logger.info(status)


class PlotlyHandler:
    ''' Create a message handler for streams
    that writes to streams that are indexed by token
    '''
    def __init__(self, stream_tokens):
        ''' Store the stream tokens in an object
         so that the message handler can access them
        '''
        self.streams = {}
        for token in stream_tokens:
            self.streams[token] = py.Stream(token)
            self.streams[token].open()

    def handlePlotlyMessages(self, msg):
        ''' Write the msg data to the associated plotly stream
        msg is a json string of the form:
            {"x": 1, "y": 1, "token": "ibagefhdci"}
            or
            {"y": 1, "token": "ibagefhdci", "timezone": "America/Montreal"}
        If it's the latter form, we'll compute the time and assign that as x
        '''
        data = json.loads(msg)
        if data['token'] in self.streams:
            token = data['token']
            del data['token']
            if 'x' not in data and 'timezone' in data:
                try:
                    tz = timezone(data['timezone'])
                except:
                    c.logger.warning("Unrecognized timezone: {timezone}\n"
                        "Defaulting to America/Montreal".format(timezone=data['timezone']))
                    c.logger.debug(traceback.format_exc())
                    tz = timezone("America/Montreal")
                del data['timezone']
                data['x'] = tz.localize(datetime.datetime.now())
            self.streams[token].write(data)
            time.sleep(50./1000.)
        else:
            c.logger.warning("Recieved the token: {token} which has no associated stream.\n"\
                "Add the token {token} to your config.json file to register this stream.".format(token=data['token']))

plotlyHandler = PlotlyHandler(stream_tokens)

# Assign a "subscriber" to `plotly` messages that are transmitted from plotly
c.onMessage['plotly'] = plotlyHandler.handlePlotlyMessages

# Start listening for messages (data) from the Arduino!
c.run()
