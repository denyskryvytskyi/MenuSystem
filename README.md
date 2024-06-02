# Multiplayer sessions plugin with menu
Plugin for handling multiplayer Unreal Engine 5 Project sessions across the Internet.
Available options:
- LAN
- Steam
  
# Getting started
To use the plugin in your project:
1. Move the `MultiplayerSessions` folder to the `Plugins` folder in the root folder of your project.
2. Enable the Online Subsystem Steam plugin for the project:
    in UE editor: Edit → Plugins:
      ![image](https://github.com/denyskryvytskyi/MenuSystem/assets/25298585/68d9bc57-1ef8-4b98-a2b1-afa2d0dd069e)
3. Add settings to the config files in the `Config` folder:
    - Steam settings in `DefaultEngine.ini`:
    ```
    [/Script/Engine.GameEngine]
    +NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="OnlineSubsystemSteam.SteamNetDriver",DriverClassNameFallback="OnlineSubsystemUtils.IpNetDriver")
    
    [OnlineSubsystem]
    DefaultPlatformService=Steam
    
    [OnlineSubsystemSteam]
    bEnabled=true
    SteamDevAppId=480
    bInitServerOnClient=true
    
    [/Script/OnlineSubsystemSteam.SteamNetDriver]
    NetConnectionClassName="OnlineSubsystemSteam.SteamNetConnection"
    ```
    - Game session settings in `DefaultGame.ini`:
    ```
    [/Script/Engine.GameSession]
    MaxPlayers=100
    ```
4. Regenerate project files: RMB on the <Project>.uproject → Generate Visual Studio project files.
5. [Optional] You can use the menu widget of the plugin to host/join sessions through the UI in the following way (e.g. Level Blueprint):
![image](https://github.com/denyskryvytskyi/MenuSystem/assets/25298585/9ce11bf7-955a-4154-b379-a98c715e128f)

   
   
