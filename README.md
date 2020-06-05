# 4D Rocket League Ball

This project is a combination of three projects. 

<ol>
<li> A 3D Printed Ball with a simple circuit controlled by Arduino</li>
<li> An arduino sketch with a custom RGB LED Library </li>
<li> A Rocket League Plugin that tracks when a goal is scored</li>
</ol>

## Setup

### 1. The Circuit

The list: 

<ul>
<li>5 Common anode RGB Leds</li>
<li>6 150 ohm ressitors</li>
<li>3 100 ohm resistors</li>
<li>Jumper Wire</li>
<li>Wemos D1 Mini (or clone)</li>
<li>Something to solder it to, I used Proto board</li>
</ul>
 
 The circuit is simple enough, here are a couple of schematics explaining it.

![schematics](/Pictures/schematics.png "My High school professor would be dissapointed with my schematic")

![DIY](/Pictures/DIYSchematics.png "This one is even worse")

Now that you made it through the MS Paint art show, we can continue with the software.

### 2. The Sketch
If you don't already have it, download the Arduino IDE: [here](https://www.arduino.cc/en/Main/Software)

There is some extra setup needed with the the arduino IDE to make it build on an ESP8266 and our D1 Mini.


1. Copy the RGBLed folder into the libraries location for the Arduino IDE. For me it was here: ```C:\Users\<USERNAME>\Documents\Arduino\libraries``` This allows us to use the custom library in the sketch.

       
2. To work with the esp8266, we need the IDE to recognise the board. In **File->Preferences**, Where it says **Additional Boards Manager URLs**, add this URL which includes the necessary info for the esp826: ```https://arduino.esp8266.com/stable/package_esp8266com_index.json```

With that taken care of, we should be able to go about editing and loading the sketch to the board. But first, I want to talk about what the code is doing.

There are really just two things happening. 

<li>The first is that the code establishes an Over the air or OTA method to update the Arduino. This allows us to update it over wifi and not need to have it hooked up to a PC in order to update it. 

<li>The second thing is that it sets up a web server and simple web page to listen for incoming calls to its IP. Depending on the URL, it will cause the Lights to act differently.

1. Modify the two #defines so that the arduino can connect to your wifi.
    ```
    #define STASSID "WIFI-NAME"
    #define STAPSK  "WIFI-PASSWORD"
    ```
2. Under **Tools->Board**, change it to **LONLIN(WEMOS) D1 R2 & mini**
3. Select the right port under **Tools->Port**, you may only have one option.
4. With the setup done, you should be able to write the code to the board. Press the Arrow in the top left to compile and write the sketch to the board.   

Some common issues might be theat the path isn't found for the library or the other headers. Just make sure you followed the previous steps. If there is another issue, reach out to me or tag an issue in the repo. Just make sure to be plugged into the computer when making major changes, because if you break the OTA feature, you have to plugin.
<br />  

Once the code is loaded, the arduino should be ready to go. The ball should be glowing, and connected to WIFI. If not, Make sure the WIFI and password are correct. There should also be a valid webpage to navigate to from your computer. You will have to find what the IP address is, or you can set it manually in the arduino sketch. I used my Router, so I didn't worry about it. 

![website example](/Pictures/Website.png "Now my College web programming professor can be dissapointed in me...")

Now that it's loaded, let's talk about how it works. The links on the page are received by the arduino and it makes the lights work. I used the simple URL method, so just GET calls will help us accomplish what we want. This could change in the future for more effects.
<br /> 

<li> **LED ON** changes all LEDS through the colors (Good color test)
<li> **LED Off ** turns off the LEDS (JK, I broke it because I added the RGB Fade effect...)
<li> **GOAL** Blinks the lights randomly for 5 Seconds
<li> **WIN** This alternates between Green and White for 5 seconds
<li> **FADE** This fades the lights through the main colors (Good for testing, but somewhat obsolete because now it happens anyway)
<li> **OTA** This button will deactivate the Over the air update system. I added this because I was under the impression that it can cause the communication from Rocket league to the arduino to fail. I am not sure, but I did this just in case. It will also speed up the fade animation so you know what mode it is in.</li> 
<br />
Use the webpage to <s>play</s> test that the system works, then we will move on to the Rocket League plugin.

### 3. The Plugin

The plugin is a dll that will get loaded into Rocket league by Bakkesmod. 

First, you need [Bakkesmod](https://bakkesmod.com/)

Then, you need the [BakkesModSDK](https://github.com/bakkesmodorg/BakkesModSDK). It should be included in the bakkesmod files already. The linked version, should however, be more up to date.

I will admit, I haven't put as much time into the plugin part, which, ironically will be the most powerful going forward to integrate into other systems. But with the included project, building and deploying shouldn't be too bad. 

1. Update the IP address in **RLBall.cpp** to match your arduino. It should be on line **39** and **53**, but could move.
2. The file **bakkes_patchplugin.py** in the BakkesModSDK folder, for me it is here: 
   ```("C:\Program Files (x86)\Steam\steamapps\common\rocketleague\Binaries\Win64\bakkesmod\bakkesmodsdk\")```
Change line **13** to your bakkesmod plugin folder. I included the file in the repo, but it is really a one line change.
4. Set the build type to **Release** and **x64**. 
5. Build the Solution. There is a post-build event to send the updated plugin to the plugin folder. If it all builds successfully, it should automatically load when Rocket league is launched or it will reload if Rocket league is already running.

Jump in a custom online game and try it out. **F6** opens the console