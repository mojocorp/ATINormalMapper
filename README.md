[![GitHub Releases](https://img.shields.io/github/release/mojocorp/ATINormalMapper.svg)](https://github.com/mojocorp/ATINormalMapper/releases)

```
------------------------------------------------------------------------------

The purpose of the NormalMapper is to generate normal (bump) maps from
a high and low resolution model.  The key files are:

bin/NormalMapper - Utility to generate normal maps.
bin/NMFView - OpenGL NMF file and normal map viewer
bin/WorldSpacifier - Utility to transform tangent space maps to world space.
bin/NMFConvert - Converter to change older NMF files into new NMF files.
doc/NormalMap.ppt - Power Point Slides from Alex Vlachos' ATI Mojo Day talk.
data/CarLow.nmf - Sample low resolution model
data/CarHigh.nmf - Sample high resolution model
data/CarNormals.tga - Sample normal map.

NMFExport.mll - Maya 4.0 exporter
Maya45NMFExport.mll - Maya 4.5 exporter
Maya50NMFExport.mll - Maya 5.0 exporter
MAXExportNMF.dle - 3DSMAX 3.1 exporter
MAX4ExportNMF.dle - 3DSMAX 4.0 exporter
MAX5ExportNMF.dle - 3DSMAX 5.0 exporter

------------------------------------------------------------------------------

The NormalMapper is the utility that actually generates the normal
map. It reads in a low resolution model and a high resolution model
that have been exported from either 3DS MAX or Maya using the
exporters given. It uses the texture coordinates from the low
resolution model to rasterize into a texture of the size given on the
command line. The normals placed in the texture are determined by
casting a ray from the low resolution model in the direction of the
interpolated normal from the low resolution mesh. At the intersection
of this ray with the high resolution model the interpolated normal
from the high resolution model is determined, this normal is placed in
the normal map directly or it can be further perturbed by a height
field placed on the high resolution model with a texture map. The
default behavior of the NormalMapper is to take the ray intersection
that lies closest to the low resolution mesh, weather that
intersection is in front of or behind the low resolution mesh. If
there are multiple intersections at equal distance the normal that is
closest to the cast ray is used. This behavior can be changed via
command line parameters. For further technical explanation see
NormalMap.ppt.

A couple of quick comments about texture coordinates and normals in
the low resolution model. The texture coordinates on the low
resolution model have a couple of restrictions: first they must lie
inthe 0-1 range and second there should be no overlap or reuse of
texture coordinates. The texture coordinates on the low resolution
model also should not contain triangles where two vertices share the
same texture coordinates, this causes singularities in tangent space.
There are no restrictions on texture coordinates for the high
resolution model. The texture coordinates in the high resolution model
are only used if the normal map is being further perturbed by a bump
map. For the best results the normals in the low  resolution model
should be fairly smooth to insure that no geometric detail is missed
that might cause lighting artifacts. 

The NormalMapper has many options available to change the way the
normal maps are generated. The following line shows the complete set
and they are described following that.

NormalMapper [-0123456789aAbBcCdDeEfFgGhHiklLmMnNoOpPqQrRtTvwXyz] lowres highres Width Height [value] [heightmap heightscale] outputname

The following options control the amount of output given from the
NormalMapper. These turn off various levels of output for use in
scripts and such.

               q  - Quiet mode, no spinners or percentages
               Q  - Very quiet mode, no messages

The following options deal with the output format for the texture. By
default the texture is output as an 24bit TARGA image. The .arg format
is an internal format used at ATI, the source files ArgFileIO.h and
ArgFileIO.cpp contain code for reading and writing these images.

               h  - Output 16x16 .arg format texture
               H  - Output 16x16x16x16 .arg format texture
               i  - Output 10x10x10x2 .arg format texture

By default there is no tolerance when doing the ray casting, meaning
there might be some floating point inaccuracy causing some visual
artifacts. The following commands increase the tolerance on the
compares.

               t  - larger tolerance on compare (1.0e-7)
               T  - largest tolerance on compare (0.1)

The default behavior of the normal mapper is to cast a single ray per
texel. The following options increase the number of samples per texel
to give some anti-aliasing.

         default  - Single sample per texel 
               1  - Five sampless per texel
               2  - Ten samples per texel
               3  - Seventeen samples per texel
               4  - Twenty six samples per texel
               5  - Thirty seven samples per texel
               6  - Fifty samples per texel
               7  - Sixty five samples per texel
               8  - Eighty two samples per texel
               9  - One hundred and one samples per texel
               0  - One hundred and twenty two samples per texel
               k  - Add four samples for texel corners

By default the NormalMapper converts the normals into the tangent
space of the low resolution model. The following option turns this off
and places the world space normals into the normal map.

               w  - Leave normals in world space

To reduce filtering artifacts a dilation filter is used to expand
regions with normals. The following command line option will turn this
feature off or increase the amount of expansion.

               e  - Don't expand border texels
         default  - expande border texels (10 texels)
               E  - expand border texels more (15 texels)
               g  - more dilation (20 texels)
               G  - even more dilation (30 texels)

An optional post process blur filter can be applied to the final
normal map image using the following command line option.

               B  - Box filter final normal map image

The NormalMapper can be used to create the entire mip-chain for the
normal map using the following two options. The first option does a
full ray cast for each mip level, the second option uses a box filter
to down sample the base level normal map image and renormalizes.

               m  - Create mip chain (recast at each mip level)
               M  - Create mip chain (box filter)

The following group of options control how the NormalMapper chooses
which, of potentially many intersections should be used as the
normal. The first two pick the normal at the intersection that is the
closest to the low resolution model. This can potentially be "behind"
the low resolution model. The difference between the two is in the
case where multiple intersections exist that are the same distance
from the low resolution model either the first is chosen (c) or the
one that most closely corresponds to the low resolution model is
chosen. The latter option is the default behavior of the
NormalMapper.

               c  - Pick closest intersection, first if equidistant
               C  - Pick closest intersection,
                    normal closer to low res if equidistant (default)

The next two options perform the opposite operation. They choose the
normal that is the furthest from the low resolution model. The lower
case and uppercase versions distinguish between intersections that lie
at the same distance from the low resolution model.

               f  - Pick furthest intersection, first if equidistant
               F  - Pick furthest intersection,
                    normal closer to low res if equidistant

The next two operations are a hybrid of the previous two. If an
intersection is found in front of the low resolution model it is
preferred over one behind the low resolution. The furthest
intersection from the low resolution model is chosen if there are any
that lie in front of the low resolution model. If no intersections are
found in front of the low resolution model the normal closest to the
low resolution model is selected. The capitalization of the option
again switches between choosing the first normal or the normal closest
to the interpolated normal in the case of equidistant intersections.

               d  - Pick furthest intersection in front,
                    closest intersection behind,
                    first if equidistant
               D  - Pick Pick furthest intersection in front,
                    normal closer to low res if equidistant,
                    closest intersection behind,

The final selection option chooses the normal that most closely
matches the low resolution model.

               b  - normal closest to low res

The following option allows the further control of only picking
intersections that lie in front of the low resolution model.

               X  - only normals in front of low resolution model

The next option allows the specification of a maximum distance away
from the low resolution model. This will cause the intersection
selection to ignore any intersections that lie further than the given
distance away from the low resolution model. It requires an additional
command line argument to be passed.

               v  - [value] is the maximum distance

The "a" and "L" options control the cutoff angle for determining if
the normal found in the high resolution is facing roughly the same
direction, effectively eliminating triangles that actually face the
low resolution model. The smaller the angle the closer the normal in
the high resolution must be to the normal in the low resolution model.

               a  - smaller angle for cutoff
               A  - smallest angle for cutoff
               l  - larger angle for cutoff
               L  - largest angle for cutoff

As part of the process of generating normals the NormalMapper by
default places a value equivilent to casting a ray from the closest
triangle edge into the border texels. It then copies this value into
the final normal map. These values then get dilated (assuming that
dilation was not turned off). To just peform the dilation and not this
edge copy use the following option.

               z  - don't perform edge copy

In an effort to try and get any potential samples that may lie in
neighboring texels the NormalMapper pads it's search around each
triangle. To have the NormalMapper pad a bit more use the following options:

               p  - more triangle padding
               P  - even more triangle padding

The NormalMapper can generate "bent normals" as well as the
traditional normal.  To do this it ray casts a hemisphere of rays out
from the intersection with the high resolution model, the rays that
don't hit any geometry are averaged and the result is the bent
normal.  This has the effect of bending the normal in the direction
where a majority of the rays escape and can be used for environment
map lighting to skew the lookup to a portion of the environment map
where the pixel would get the most light contribution. If either of
the following two options is set the normal placed into the map is
this bent normal.

               n  - Generate bent normal (261 rays)
               N  - Generate bent normal (581 rays)

The NormalMapper can also generate an occlusion term per texel.  This
term is computed in a similar manner to the bent normal. A hemisphere
of rays is cast from the intersection with the high polygon model, the
percentage of rays that "escape" are stored to give an occlusion
term. This term can be used for ambient lighting to darken up areas
that are self shadowed. If either of the following command line
options is present this occlusion term is stored in the alpha channel
of the normal map.

               r  - Generate occlusion term (69 rays)
               R  - Generate occlusion term (149 rays)
               o  - Generate occlusion term (261 rays)
               O  - Generate occlusion term (581 rays)

The NormalMapper can keep track of the displacement along the normal
while it is generating the map. It will write this to a .arg file if
the following option is set.
    
               y  - keep track of displacement values

As an example of using the NormalMapper, a low resolution and high
resolution model have been provided (CarLow.nmf CarHigh.nmf). The
command line used to generate these models is shown below:

NormalMapper -o CarLow.nmf CarHigh.nmf 1024 1024 CarNormals.tga

The file CarNormals.tga is also included.

An example of further perturbing by a bump map is also included with
the model files: AddedBumpL.nmf, AddedBumpH.nmf and bump map file
Addedbump.tga. The output is included as AddedBumpoutput.tga. This
file was generated with the following command line:

NormalMapper AddedBumpL.nmf AddedBumpH.nmf 1024 1024 Addedbump.tga 1 AddedBumpOutput.tga

In this case the beginning of the command line is similar to the
previous example and contains the names of the low resolution
(AddedBumpL.nmf), high resolution (AddedBumpH.nmf) as well as the
width (1024) and height (1024) of the output normal map. The next two
parameters are the name of the height field for further perturbing the
normal map and a scale to increase or decrease the severity of the
bumps generated by the height field. The final argument is the name
for the output normal map.

------------------------------------------------------------------------------

To view the results of the NormalMapper a small GLUT based application
is included: NMFView. When you start this application it brings up a
file open dialog box for the model file (.nmf file) and then one for
the normal map file (.tga file). The default view shows diffuse bump
mapping using the normal map. This can be changed to show the normal
map texture or GL vertex lighting on the low resolution normals using
the a key. The rest of the keyboard commands are shown below.

a              - Cycle showing normal map, diffuse bump, or vertex lit
b              - Cycle background color
c              - Toggle converting light vector to tangent space
l              - Load a new object and normal map
n              - Toggle display of normals
r              - Reset view/light position
t              - Toggle display of tangent/binormal
w              - Toggle wireframe
ESC or q       - Quit program

The model can be moved around using a combination of mouse button
presses and movement. The light is fixed at the viewer's location.

LEFT + MOUSE   - Rotate
MIDDLE + MOUSE - Pan
RIGHT + MOUSE  - Zoom

------------------------------------------------------------------------------

The WorldSpacifier is a utility that converts a tangent space normal
map into a world space normal map. It takes an nmf model and either a
normal map or a height/bump map and converts it into world space. The
command line options are as follows:
 
WorldSpaceifier [-hHimMBeEgGpPqQ] model normal/bumpmapName [heightscale] outputname\n";

The model needs to be a .NMF file with texture coordinates. If using a
normal map, no heightscale is provided. If using a bump map the
heightscale determines how high a maximum value should be when
converting the height field into normals. The outputname determines
where to save the resulting file. There are a number of options to
control the process, many of them are similar to the NormalMapper
utility. 

The first block of command line options allow you to control the
output format for the normal map.

                     h  - Output 16x16 .arg texture
                     H  - Output 16x16x16x16 .arg texture
                     i  - Output 10x10x10x2 .arg texture

You can also automatically create the mip chain for the normal map
using the following options.

                     m  - Create mip chain (remap)
                     M  - Create mip chain (box filter)

You can run a box filter over the resultant image when done.

                     B  - Box filter final image

You can control the dilation of the image for reducing bilinear
filtering artifcats.

                     e  - Don't expand border texels
                     E  - expand border texels more (15 texels)
                     g  - more dilation (20 texels)
                     G  - even more dilation (30 texels)

Triangle pad helps to reduce the seams between triangles. Similar to
what happens within the normal mapper.

                     p  - more triangle padding
                     P  - even more triangle padding

You can also control how many messages get printed out to show progress.

                     q  - Quiet mode, no spinners or percentages
                     Q  - Very quiet mode, no messages

An example of using the Worldspaceifer is shown below:

WorldSpaceifier 

------------------------------------------------------------------------------

In order to use any of the Maya Exporters for NMF files the *NMFExport.mll
file needs to either be placed in your Maya plug-ins directory or the
directory where you unzipped the NormalMapper needs to be added to
your MAYA_PLUG_IN_PATH environment variable. To enable the plug-in you
will need to go to Window | Settings/Preferences | Plug-in Manager
menu item. This will bring up the plug in manager window. Click on the
check box for both Loaded and Autoload for NMFExport. Then restart
Maya. You should now be able to select meshes and use the File |
Export Selection option choosing NMF format.

To use the MAX Exporters copy the appropriate version of the
MAXExportNMF.dle file into your 3DS MAX plugins directory. When you
start MAX and select an object you will then be able to use the File |
Export Selected menu item to export NMF files.

The source code for the various utilities is provided along with
Visual C++ project files. The NormalMapper and NMFView programs should
build as is, the exporters require that the path to the Maya or 3DS
MAX SDKs be in your library and include paths.
```