
This app will serve as an example Android app that uses the ARIA and
ArNetworking libraries, and be a useful utility for controlling a
server application on the robot which is using ARIA and/or ARNL or MOGS.


The following are plans for the ARIA and ArNetworking libraries:
   * Eventually all Android-specific changes should be merged into main Aria ad
     ArNetworkig libraries, and build system added there as well.  The 
     app described below would just build this if neccesary in /usr/local/Aria
     and link to it from there.
   * Someday add ability for ARIA to connect to robot controller over Bluetooth (if
     robot has a bluetooth-serial adapter).

The following are ideas for what the App should do.  Items in double parentheses
((like this)) are lower priority and could be done in the future.

All views should display server mode and server status.

Activities, access via tabs or other mechanism:
    * Connect
        * Trigger Android wifi connection menu (either always or if not o a wifi
          network)
        * Enter IP address or host name of server, or select from a list
        * ((Store list of recently used addresses))
        * ((Use service discovery to find servers runninng on the same network))
        -> How? Use ArClientBase to connect.
    * General status and menu of actions/modes
        * Display robot state updates (X, Y, Th, Vel, Battery [volts or %], etc.
          -> How? Use ArClientHandlerRobotUpdate to request and receive info
        * Buttons for major modes:
          * Dock
            -> requestOnce("dock")
          * ((Home))
            -> requestOnce("home")
          * ((Wander))
            -> requestOnce("wander")
          * ((Tour Goals))
          * Start/Stop Mapping (Toggle)
            -> request "mappingStart" and "mappingEnd".  Receive "mappingStatusBroadcast" messages to set state of toggle
          * ((Localize to Dock?? Localize to Home??))
      
    * Drive/Teleoperate
        * Drag to teleoperate the robot
    * Goals
        * Download map if available, list goals from the map, click to request
          ARNL to go to that goal
    * Commands
          * Everything from "custom commands"
          -> get list with listCommands and listStringCommands requests
    * Detailed Info
        * Display server mode and status (use ArClientHandlerRobotUpdate)
        * Display info from "update" (X, Y, Th, Vel, Battery [volts or %], etc. (use ArClientHandlerRobotUpdate)
        * Display table of everyhing from String Info
          -> get list by requesting "getStringsInfo", get values by requesting "getStrings"
    * ((Config))
    * ((Log))
    * ((Map with drawings and localize to point))

Misc ideas:
  * Should show popup dialogs sent from server
  * Send user an Android Notification on certain events such as low battery
    warning, disconnection, server popups, error-level log messages.


