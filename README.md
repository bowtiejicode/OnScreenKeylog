# OSK: On-Screen Keylogger

Traditional user-land keyloggers usually perform Windows Hooking (SetWindowsHookEx) or Polling (GetAsyncKeyState, GetKeyboardState) to capture the keystrokes. These methods have one common hurdle to overcome; which is User Interface Privilege Isolation (UIPI). This also means that if your keylogger is running with normal rights (medium integrity level), there is no way to capture keystrokes being entered to a higher privilege process (i.e., high/system integrity level). However, if your keylogger has UiAccess flag enabled, you are able to overcome this issue.

When it comes to UiAccess, most people will think of UI Automation (e.g., automatically sending keystrokes/mouse inputs to application). However, by having UiAccess rights, you can go [beyond sending inputs to interfaces](https://medium.com/@im-jqueue/uiaccess-and-uipi-beyond-sending-inputs-to-interfaces-db9efbd8b339).

According to this [article from Microsoft](https://learn.microsoft.com/en-us/windows/security/threat-protection/security-policy-settings/user-account-control-only-elevate-uiaccess-applications-that-are-installed-in-secure-locations#vulnerability), if your application (keylogger) has UiAccess flag enabled, you have the capability to read input for all integrity levels by using low-level hooks, raw input, GetKeyState, GetAsyncKeyState, and GetKeyboardInput. 


## Overview
There are [rules](https://learn.microsoft.com/en-us/windows/security/threat-protection/security-policy-settings/user-account-control-only-elevate-uiaccess-applications-that-are-installed-in-secure-locations#reference) to follow in order to launch your proccess the UiAccess flag. Although one can perform [token stealing](https://www.tiraniddo.dev/2019/02/accessing-access-tokens-for-uiaccess.html) to give your process UiAccess flag, we can also do this through [Mock Directories](https://www.bleepingcomputer.com/news/security/bypassing-windows-10-uac-with-mock-folders-and-dll-hijacking/) +  DLL hijacking of the built-in on-screen keyboard (osk).

>What is not well-known is that Mock Directories (originally used for UAC bypass) can actually help us fulfil the 2nd requirement (secure directories) for running UiAccess applications! 😉

### Usage
1. Build dllmain.cpp and rename the DLL as OskSupport.dll
2. Create a mock directory using powershell:
```New-Item "\\?\C:\Windows \System32" -ItemType Directory```
3. Copy osk.exe from ```C:\Windows\System32``` to ```C:\Windows \System32``` <-notice the whitespace after "Windows"
4. Copy the malicious OskSupport.dll to the mock directory ```C:\Windows \System32```
5. Run osk.exe from the mock directory ```C:\Windows \System32```
6. Keystrokes will be logged to keylog.txt

### Demo
You can only keylog on processes in the same desktop session. By default, elevation prompts (UAC) are spawned in the secure desktop, which means that you can't keylog the credentials. 

In the following demo, I have configured the environment such that elevation prompts are spawned in the same interactive desktop instead of the secure desktop. 
![alt text](a.gif)

There is a [hurdle](https://learn.microsoft.com/en-us/windows/security/threat-protection/security-policy-settings/user-account-control-allow-uiaccess-applications-to-prompt-for-elevation-without-using-the-secure-desktop) that I can't seem to overcome, but maybe you can 😃.

### Disclaimer
This project is intended solely for **educational purposes only** and in no manner supports any illegal activities. It is to showcase what can be done when your keylogger has UiAccess flag enabled. However, you may modify/enhance the code to further explore UiAccess capabilities (e.g., sending keystrokes to higher privileged GUI to perform privilege escalation OR injecting osksupport.dll into ctfmon.exe through SetWindowsHookEx so that osk.exe is not in the process list). 

I have also written a short article [here](https://medium.com/@im-jqueue/uiaccess-and-uipi-beyond-sending-inputs-to-interfaces-db9efbd8b339) to showcase how you can perform DLL injection (as an Administrator) from a medium integrity process to a high integrity process.


