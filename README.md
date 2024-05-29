User Guide <br />
<br />
The .exe file to start the program is located within the ‘/Executable’ (works on windows only). <br />
<br />
<br />
Controls__
/n• w,a,s,d,q,e – Move the camera.__
• L/R Arrows – Rotate camera.__
• Mouse – Rotate camera.__
• CTRL – Open/Shut UI.__
__
UI Controls__
__
Spline File:__
• File path - spline file to load (I have provided example splines in /data/splines), if the spline __
file does not exist it will create a new spline.__
• Load/Create – Loads/create the spline file entered above.__
• Save – Saves spline data to file.__
• Undo – loads data from last save.__
__
Track Mesh:__
• Update Model Fast – Creates/update an object along the spline from a template file.__
• Update Model Detailed – Creates a very detailed version of the model along the spline (this is very slow and should only be used if you are planning on using the model in other programmes like Unity or Blender).__
• Wrap – Make the object travel along the full length of the spline (removes small gap at end).__
• Add/Remove Normal – Adds/remove vertex normal from generated object.__
• Template Path – The object file that should be mapped along the spline.__
• Gen Path – The name/location of the new .obj file of the object that will be created.__
• Axis – Template object’s forward-facing direction.__
• Scale – Scale of generated object compared to the template object.__
• Detail – The error allowed when mapping the object to the spline (this effects performance and setting it too high might break the created object).__
• Offset – Distance the generated object should be from the spline.__
__
Spline Nodes:__
• Index – Select node.__
• X,Y,Z – Node’s position .__
• Add Node – Adds a new node at current index.__
• Remove Node – Removes currently selected node.__
__
Role Nodes:__
• Index – Select roll node.__
• Pos – Roll nodes position on spline.__
• Angle – Angle of roll.__
• Add Roll Node – Adds roll node at current index.__
• Remove Roll Node – Removes currently selected roll node.__
• Sort Roll Nodes – Sorts roll nodes in order of position on spline.__
• Wrap/Unwrap – Makes the start and end of the spline match roll angle.__
__
Boosters:__
• Index – Select booster.__
• Start/end – Start and end point of the booster on the spline.__
• Speed – The minimum speed the animated object will move over the booster when in physics mode.__
• Add Booster – Add new booster along spline.__
• Remove Booster – Removes currently selected booster.__
__
Spline Controls:__
• Degree – Sets degree of the spline (how many nodes per Bezier curve + 1).__
• Make Loop – Add nodes to the end of the spline to make it a perfect loop (clicking more than once will add too many nodes and break the loop).__
• Clamp/Unclamp – Locks the start and end of the spline to the first and last nodes.__
__
Animation Object Controls:__
• Step/Physics Mode – Change the animated mode between a physics mode and a fixed value step mode.__
• Pos – Object’s position on the spline.__
• Step Size – Fixed speed to move along the spline.__
• Mass – Objects mass.__
• Drag – Objects drag.__
__
View Controls:__
• Toggle what is shown and the colour of the spline elements.__
__
Spline Models__
I have provided two models that can be mapped along the spline these are:__
• models/track/coasterTrackShort.obj (scale = 0.002)__
• models/track/Rail.obj (scale = 0.5)__
__
Loading the files might take a over 30 seconds depending on the size of the spline!
