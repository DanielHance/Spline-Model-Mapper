User Guide

The .exe file to start the program is located within the ‘/Executable’ (works on windows only).


Controls
• w,a,s,d,q,e – Move the camera.
• L/R Arrows – Rotate camera.
• Mouse – Rotate camera.
• CTRL – Open/Shut UI.

UI Controls

Spline File:
• File path - spline file to load (I have provided example splines in /data/splines), if the spline 
file does not exist it will create a new spline.
• Load/Create – Loads/create the spline file entered above.
• Save – Saves spline data to file.
• Undo – loads data from last save.

Track Mesh:
• Update Model Fast – Creates/update an object along the spline from a template file.
• Update Model Detailed – Creates a very detailed version of the model along the spline (this is very slow and should only be used if you are planning on using the model in other programmes like Unity or Blender).
• Wrap – Make the object travel along the full length of the spline (removes small gap at end).
• Add/Remove Normal – Adds/remove vertex normal from generated object.
• Template Path – The object file that should be mapped along the spline.
• Gen Path – The name/location of the new .obj file of the object that will be created.
• Axis – Template object’s forward-facing direction.
• Scale – Scale of generated object compared to the template object.
• Detail – The error allowed when mapping the object to the spline (this effects performance and setting it too high might break the created object).
• Offset – Distance the generated object should be from the spline.

Spline Nodes:
• Index – Select node.
• X,Y,Z – Node’s position .
• Add Node – Adds a new node at current index.
• Remove Node – Removes currently selected node

Role Nodes:
• Index – Select roll node.
• Pos – Roll nodes position on spline.
• Angle – Angle of roll.
• Add Roll Node – Adds roll node at current index.
• Remove Roll Node – Removes currently selected roll node.
• Sort Roll Nodes – Sorts roll nodes in order of position on spline.
• Wrap/Unwrap – Makes the start and end of the spline match roll angle.

Boosters:
• Index – Select booster.
• Start/end – Start and end point of the booster on the spline.
• Speed – The minimum speed the animated object will move over the booster when in 

physics mode.
• Add Booster – Add new booster along spline.
• Remove Booster – Removes currently selected booster.

Spline Controls:
• Degree – Sets degree of the spline (how many nodes per Bezier curve + 1).
• Make Loop – Add nodes to the end of the spline to make it a perfect loop (clicking more than once will add too many nodes and break the loop).
• Clamp/Unclamp – Locks the start and end of the spline to the first and last nodes.

Animation Object Controls:
• Step/Physics Mode – Change the animated mode between a physics mode and a fixed value step mode.
• Pos – Object’s position on the spline.
• Step Size – Fixed speed to move along the spline.
• Mass – Objects mass.
• Drag – Objects drag.

View Controls:
• Toggle what is shown and the colour of the spline elements.

Spline Models
I have provided two models that can be mapped along the spline these are:
• models/track/coasterTrackShort.obj (scale = 0.002)
• models/track/Rail.obj (scale = 0.5)

Loading the files might take a over 30 seconds depending on the size of the spline!
