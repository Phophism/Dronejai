## Real-time Graphing and Data Logging with the Yún

#### The easiest and fastest way to plot and share data on the Yún.


The Plotly-Yún library streams data from your Arduino Yún to your Plotly web-account. It's free, open source, and the graphs and data are entirely online.

<p align="center">
<a href="http://plot.ly/~streaming-demos/6">
<img src="https://raw.github.com/plotly/Streaming-Demos/master/readme_gifs/real-timesensor.gif">
</a><br>
<a href="http://plot.ly/~streaming-demos/6">Click through to see the live graph</a>
</p>

#### It's super easy

```C
#include <PlotlyYun.h>
#include <YunMessenger.h>
#include <Bridge.h>
#include <Console.h>

// Initialize plotly "plotters" with a unique token that is generated for you
// when you sign up for a free account on plotly (https://plot.ly)
// Find your stream tokens in your plotly account here: https://plot.ly/settings
// Initialize as many plotters as you want! Each plotter will send
// data to the same plot as a separate line.

plotly plotter("put_your_10_character_stream_token_here");

void setup() {

    // start-up the bridge and the console
    Bridge.begin();

    delay(2000);
    Console.begin();
    while (!Console) {
      ; // wait for Console port to connect.
    }
    Console.buffer(64);
    delay(2000);
}

void loop() {

    plotter.plot( analogRead(A0) );

    delay(100); // Graph 10 points every second
}
```



## 5-Min Quickstart


#### Connect your Yún to WiFi
You know the drill:

1. Reset the WiFi chip: hold down the WLAN button for ~10 seconds until Arduino Yun-XXXX comes up as a network. I've found that this sometimes takes multiple tries and may take ~45 seconds.
2. Connect to that network, enter `arduino.local` into your browser
3. Configure the network, making sure that:
 - Your encryption (WPA, WPA2, WEP) is correct
 - Your *case-sensitive* network name is correct
4. Connect to your old network, go to `arduino.local` in your browser. I've found that sometimes this may take ~30 seconds. If your browser can't find it, go back to step 1!

#### Set up the Linino Side
1. Download and uncompress this folders's zip file: [https://github.com/plotly/arduino-api/raw/master/plotly_yun/plotly_yun.zip](https://github.com/plotly/arduino-api/raw/master/plotly_yun/plotly_yun.zip)
2. Update the file called `config.json` in the `Linino` folder with your plotly credentials. You can sign up to plotly here (it's free!): [https://plot.ly/ssu](https://plot.ly/ssu) and you can view your API key and stream tokens here: [https://plot.ly/settings](https://plot.ly/settings).
    Your filled-in `config.json` file will look like:

   ```json
   {
        "plotly_username": "anna.lyst",
        "plotly_api_key": "ab4kftunvd",
        "plotly_streaming_tokens": ["ab4kf5nfdn", "kdf5bn4dbn"],
        "show_this_many_points": 500,
        "file_option": "overwrite"
   }
   ```

   (those are fake keys and tokens, so don't try to use them, they won't work!)
3. Open up your computer's terminal and move to where you downloaded this folder:

    ```bash
   $ cd ~/Downloads/plotly_yun
   $ ls
    Arduino         Linino          README.md
   ```


4. Copy all of the files from the Linino folder to your Linino

    ```bash
    $ scp -r Linino/* root@arduino.local:/root/
    ```

   a TON of stuff will print out, don't worry! If it is your first time connecting to the device, you may recieve the message:
   ```
    $ scp -r Linino/* root@arduino.local:/root/
    The authenticity of host 'arduino.local (192.168.2.17)' can't be established.
    RSA key fingerprint is ba:fe:ce:d6:59:33:00:be:af:9c:9e:f6:df:2q:a8:8d.
    Are you sure you want to continue connecting (yes/no)?
    ```
    No need for alarm, just type `yes`!
5. Cruise into your Linino and install python-openssl:

    ```bash
   $ ssh root@arduino.local
   root@Arduino:~# opkg update
   root@Arduino:~# opkg install python-openssl
   ```

(Are you new to the terminal? Here is screen-cast video of my terminal as I run through commands 3-5:
[![](http://new.tinygrab.com/c751bc2ee2a578abe66d6b4d045e683ec084679f39.png)](http://asciinema.org/a/8137)

Still need some help? Get in touch!)

#### Run the Arduino program
1. Add the folders inside the `Arduino` to your Arduino libraries folder. On a Mac, this is in `~/Documents/Arduino/libraries/`
    ![](http://new.tinygrab.com/c751bc2ee2bdecc21b412dd36ba491606ed81fd36a.png)
2. Open up the [latest version of the Arduino IDE](http://arduino.cc/en/main/software)
3. Load up example.ino from the `Arduino` folder in this repository and add your tokens to the script, i.e. change these lines:

    ```cpp
    plotly plotter1("put_your_10_character_stream_token_here");
    plotly plotter2("put_another_token_here");
    ```

    to the tokens you that you placed in `config.json` and that you found in [https://plot.ly/settings](https://plot.ly/settings):

    ```cpp
    plotly plotter1("ab4kf5nfdn");
    plotly plotter2("kdf5bn4dbn");
    ```

4. Make sure that your IDE has selected `Arduino Yun` and the remote port:
![Arduino IDE Port Selection](http://new.tinygrab.com/c751bc2ee2ee7fe227a406dbdb8a3d6dfb53b00b29.png)
5. Upload the program!
6. Go back into your Linino, and run `run.py`. It'll take about 10-45 seconds before the output comes up:
    ```bash
    $ ssh root@arduino.local
    root@Arduino:~# python run.py
    Plot initialized at: https://plot.ly/~Arduino-Yun/28
    with tokens: ab4kf5nfdn, kdf5bn4dbn
    ```

   (your tokens and your URL will be different!)

   If you're watching the logs, you'll see something like:
   ```bash
   root@Arduino:~# tail -f YunMessenger.log
   2014-03-12 02:57:36 INFO: Logger initiated
   2014-03-12 02:57:38 INFO: Plot initialized at: https://plot.ly/~Arduino-Yun/28
   with tokens: ab4kf5nfdn, kdf5bn4dbn
   2014-03-12 02:57:38 INFO: Run initiated
   2014-03-12 02:57:39 INFO: Attempting to connect to localhost:6571
   ```

7. Grab the URL that the terminal printed out, view your graph in your browser, and celebrate! The graph and data will also be saved in your plotly account. You can view, edit, and share your graphs while data is streaming to them in real-time!



# More on Usage
#### Debugging and Log Files
On the Linino, status updates, warnings, and errors are written to a file called `~/Plotly.log`. This file is capped at 0.5MB.
```bash
root@Arduino:~# cat ~/Plotly.log
[...]
```

The file might have a ton of lines and be overwhelming to look at, so to check out just the last 50 lines, run:
```
root@Arduino:~# tail -n50 ~/Plotly.log
[...]
```

To watch the messages as they're being written to the file in real-time, run:
```
root@Arduino:~# tail -f ~/Plotly.log

2014-03-12 01:35:35 INFO: Attempting to connect to localhost:6571
2014-03-12 01:35:35 ERROR: Can't connect to localhost:6571
2014-03-12 01:35:35 DEBUG: Traceback (most recent call last):
  File "/root/YunMessenger/Console.py", line 83, in run
    self.console.connect(('localhost', 6571))
  File "/usr/lib/python2.7/socket.py", line 224, in meth
    return getattr(self._sock,name)(*args)
error: [Errno 146] Connection refused

2014-03-12 01:35:36 INFO: Attempting to connect to localhost:6571
2014-03-12 01:35:36 INFO: Connected to localhost:6571
```


### Overwriting or Extending Data
By default, the initialization of your graph in `run.py` overwrites the existing graph with your new data. This is the perfect option for development: when you re-run your script, a fresh new graph is created. However, when you run your Arduino for an extended period of time, the Arduino may reset itself, which would in turn reset the graph and remove the existing data. To prevent this from happening, you can use the fileopt "extend", which will append your new data to the existing data in the graph.

To do this, change the `file_option` field in `config.json` from `overwrite` to `extend`, i.e. your `config.json` file should look like:

   ```json
   {
        "plotly_username": "anna.lyst",
        "plotly_api_key": "ab4kftunvd",
        "plotly_streaming_tokens": ["ab4kf5nfdn", "kdf5bn4dbn"],
        "show_this_many_points": 500,
        "file_option": "extend"
   }
   ```

### Plotting more than 2 traces
You can plot as many traces as you'd like. You just need to initialize a unique plotly object for each stream inside the Arduino code (e.g. `plotly plotter3("third_stream_token_here")`) and add that token to your `config.json` file in the Linino.


# How does it work?

1. The Arduino microcontroller (ATmega32u4) sends data to the Linino over the `bridge`
with the [YunMessenger](https://github.com/plotly/YunMessenger) library.
The `bridge` is just a TCP connection over port `6571` on `localhost` and the
`YunMessenger` library formalizes communication over that socket and allows
multiple subscribers to listen to messages coming from the Arduino (the 32u4).

2. On the Arduino side, the communication over this socket looks like:
  ```C
  #include <Bridge.h>
  #include <Console.h>

  Bridge.begin();
  Console.begin();
  while (!Console) {
    ; // wait for Console port to connect.
  }

  Console.print(char(29)); // indicate the start of the subscriber name
  Console.print("plotly"); // the name of the subscriber
  Console.print(char(30)); // indicates the start of the message
  Console.print("{\"x\": 1, \"y\": 10}"); // the message (in this case a plotly formatted JSON object)
  Console.print(char(31)); // indicates the end of the message
  ```

3. On the Linino side, `YunMessenger.Console` opens a TCP socket on port `6571`
   of `localhost`.

4. `run.py` assigns a message handler to `"plotly"` messages that come through the
   YunMessenger. That assignment looks like:
   ```
   # Assign a "subscriber" to `plotly` messages that are transmitted from plotly
   c.onMessage['plotly'] = plotlyHandler.handlePlotlyMessages
   ```
5. These messages are parsed and sent over a TCP socket to plotly's
   streaming servers, at `stream.plot.ly`. More about plotly's streaming API
   and the Python client can be found here:
  [https://plot.ly/python/streaming](https://plot.ly/python/streaming/).

This interface allows you to signal process your data in Python (instead of C!)
before it gets sent to your graph in Plotly. So, edit `run.py`: normalize your
signal, or run a moving average over it, or execute an FFT!


# Get in touch
Need some help? Something not look right? Want to contribute? Drop us a line:
- <chris@plot.ly>
- [@plotlygraphs](https://twitter.com/plotlygraphs)


# Troubleshooting
- Get in touch!
- Connecting the yun to WiFi has no error handling. It can fail silently while attempting to connect to a network, so double check your encryption and your network name. Also, network names are case-senstive!
- Use the latest Arduino IDE (1.5.6 -r2)
- Reseting the network ("hold down the wifi button for > 5 seconds") often requires several tries!
- To verify that the python client on the Linino is ready to receive commands, run `$ telnet localhost 6571`
- Don't forget:
  ```
  while (!Console) {
    ; // wait for Console port to connect.
  }
  ```
- The first time you connect to your Linino, you may receive the message:

  ```
  $ scp -r Linino/* root@arduino.local:/root/
  The authenticity of host 'arduino.local (192.168.2.17)' can't be established.
  RSA key fingerprint is ba:fe:ce:d6:59:33:00:be:af:9c:9e:f6:df:2q:a8:8d.
  Are you sure you want to continue connecting (yes/no)?
  ```
  No need for alarm, just type `yes`!
- If you connect your Yun to different networks, you may get this message when you try to `scp` or `ssh` into the device:

  ```
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
  Someone could be eavesdropping on you right now (man-in-the-middle attack)!
  It is also possible that a host key has just been changed.
  The fingerprint for the RSA key sent by the remote host is
  ba:ge:fe:d8:53:99:14:de:af:9c:9e:g7:df:2b:a8:8d.
  Please contact your system administrator.
  Add correct host key in /Users/chris/.ssh/known_hosts to get rid of this message.
  Offending RSA key in /Users/chris/.ssh/known_hosts:61
  RSA host key for arduino.local has changed and you have requested strict checking.
  Host key verification failed.
  ```
  No need for alarm, just remove the entire line in that `known_hosts` file. For my case above, I would remove line `61` in `/Users/chris/.ssh/known_hosts` (the file location and line number are given by this line: "`Offending RSA key in /Users/chris/.ssh/known_hosts:61`").
