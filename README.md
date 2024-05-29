User Guide <br />
<br />
The .exe file to start the program is located within the ‘/Executable’ (works on windows only). <br />
<br />
<br />
Controls<br />
• w,a,s,d,q,e – Move the camera.<br />
• L/R Arrows – Rotate camera.<br />
• Mouse – Rotate camera.<br />
• CTRL – Open/Shut UI.<br />
<br />
UI Controls<br />
<br />
Spline File:<br />
• File path - spline file to load (I have provided example splines in /data/splines), if the spline file does not exist it will create a new spline.<br />
• Load/Create – Loads/create the spline file entered above.<br />
• Save – Saves spline data to file.<br />
• Undo – loads data from last save.<br />
<br />
Track Mesh:<br />
• Update Model Fast – Creates/update an object along the spline from a template file.<br />
• Update Model Detailed – Creates a very detailed version of the model along the spline (this is very slow and should only be used if you are planning on using the model in other programmes like Unity or Blender).<br />
• Wrap – Make the object travel along the full length of the spline (removes small gap at end).<br />
• Add/Remove Normal – Adds/remove vertex normal from generated object.<br />
• Template Path – The object file that should be mapped along the spline.<br />
• Gen Path – The name/location of the new .obj file of the object that will be created.<br />
• Axis – Template object’s forward-facing direction.<br />
• Scale – Scale of generated object compared to the template object.<br />
• Detail – The error allowed when mapping the object to the spline (this effects performance and setting it too high might break the created object).<br />
• Offset – Distance the generated object should be from the spline.<br />
<br />
Spline Nodes:<br />
• Index – Select node.<br />
• X,Y,Z – Node’s position .<br />
• Add Node – Adds a new node at current index.<br />
• Remove Node – Removes currently selected node.<br />
<br />
Role Nodes:<br />
• Index – Select roll node.<br />
• Pos – Roll nodes position on spline.<br />
• Angle – Angle of roll.<br />
• Add Roll Node – Adds roll node at current index.<br />
• Remove Roll Node – Removes currently selected roll node.<br />
• Sort Roll Nodes – Sorts roll nodes in order of position on spline.<br />
• Wrap/Unwrap – Makes the start and end of the spline match roll angle.<br />
<br />
Boosters:__
• Index – Select booster.<br />
• Start/end – Start and end point of the booster on the spline.<br />
• Speed – The minimum speed the animated object will move over the booster when in physics mode.<br />
• Add Booster – Add new booster along spline.<br />
• Remove Booster – Removes currently selected booster.<br />
<br />
Spline Controls:<br />
• Degree – Sets degree of the spline (how many nodes per Bezier curve + 1).<br />
• Make Loop – Add nodes to the end of the spline to make it a perfect loop (clicking more than once will add too many nodes and break the loop).<br />
• Clamp/Unclamp – Locks the start and end of the spline to the first and last nodes.<br />
<br />
Animation Object Controls:<br />
• Step/Physics Mode – Change the animated mode between a physics mode and a fixed value step mode.<br />
• Pos – Object’s position on the spline.<br />
• Step Size – Fixed speed to move along the spline.<br />
• Mass – Objects mass.<br />
• Drag – Objects drag.<br />
<br />
View Controls:<br />
• Toggle what is shown and the colour of the spline elements.<br />
<br />
Spline Models<br />
I have provided two models that can be mapped along the spline these are: <br />
• models/track/coasterTrackShort.obj (scale = 0.002)<br />
• models/track/Rail.obj (scale = 0.5)<br />
<br />
Loading the files might take a over 30 seconds depending on the size of the spline!
