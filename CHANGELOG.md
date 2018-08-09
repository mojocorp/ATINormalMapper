# Changelog
All notable changes to this project will be documented in this file.

## Changes in v03.02.02
 - Fix a bug with the octree code where planes lying exactly on octree
   cells could get discarded and cause incorrect normals
 - Fix a bug where a triangle plus the pad could step outside the
   texture bounds causing really slow performance
 - Added MAC version of the Maya exporter
 - Added MAC versions of the utility programs

## Changes in v03.02.01
 - Added exporter for MAX 6.
 - Added a lightly tested NormalMapCombiner, which lets you take two
   normal maps and combine them based on the direction of the first
   normal map.

## Changes in v03.02.00
 - Ported to the Mac. There is now a mac directory that contains the
   executable and project file needed to build the normal mapper for
   the Mac.
 - Added a new tool, WorldSpacifier, to convert a tangent space normal
   map into a world space normal map, given a .nmf model. See
   documentation below.

## Changes in v03.01.04
 - Added some code to compute better values for texture boundaries
   mainly useful for world space normal maps. Added a new option -z to
   turn off doing this computation. 
 - Added new options -p and -P to control the amount of texel padding around
   each triangle.
 - Changed number of samples in each option slightly. Also changed back
   to having the default be 1 sample per texel and -1 gives you 5 samples.
 - Added -y option to generate a 16bit .arg file with the displacements
   along the normals. The values placed in the map are placed in -1 to 1
   range by dividing by the maximum displacement (absolute value) which is
   reported when the displacement map is written. This feature isn't
   heavily tested.
 - Added -r and -R options to generate ambient occlusion with fewer rays.
 - Added -k option to add texel corners to super-sampling for whichever
   mode is selected. This will add 4 samples to every mode.
 - Added -E, -g,  and -G options to add greater dilation 15, 20, and 30
   texels respectively.

## Changes in v03.01.03
 - Changed the way super sampling works. The number of samples ramps up
   more quickly with the -# options. See documentation below. The
   pattern of samples has changed as well. Also without any command
   line options the NormalMapper now does 5 samples per texel.
 - Changed the way texels get filled slightly by checking more texels
   on the edge of each triangles for samples that may lie within the
   triangle boundaries. This should reduce some of the issues seen
   between texture borders.
 - Added a "c" option to NMFView to let you toggle converting light
   vector into tangent space in order to view normal maps generated in
   world space.

## Changes in v03.01.02
 - Added -l and -L options to widen acceptance angle.
 - Fixed a bug with -d and -D options not correctly prefering the
   normals in front of the low resoultion model to those behind
 - Removed old visibility code
 - Added some debug output for intersection tests.

## Changes in v03.01.01
 - Added -q (quiet) and -Q (silent) options
 - Fixed a bug that was causing the octree search to go on longer than
   it should causing very slow processing in some cases.

## Changes in v03.01.00
 - Added exporter for Maya 5.0
 - Changed "visibility" to use an octree, this should speed things up
   some what especially for really low resolution models where the
   previous visibility code didn't help very much.
 - Added ambient occlusion term and bent normal calculation. Command
   line options:
       -o For 261 ray occlusion term
       -O For 581 ray occlusion term
       -n For 261 ray bent normal
       -N for 581 ray bent normal
   Bent normal replaces the usual normal. Ambient occlusion term is
   placed in the alpha channel of the texture.
 - CarNormals.tga contains an occlusion term.
 - CarHigh.nmf now has the full geometry, ambient occlusion looks odd
   if the full car isn't present.
 - An additional mode added into NMFView to show the alpha channel of a
   texture if one exists. Useful for visualizing ambient occlusion term.
 - Fixed a minor bug with tangent space comparison.
 - Added some experimental code for merging tangent space differently.

## Changes in v02.02.03
 - Shrunk the internal images from 4 to 3 components since that's all
   that was being used.

## Changes in v02.02.02
 - Changed NMF file format to be more robust.
 - Added a conversion program NMFConvert to change old NMF files into
   new ones. Usage: NMFConvert input.nmf output.nmf
 - Cleaned up memory usage for visibility, should use much less memory now.
 - Added Exporter for Maya 4.5.

## Changes in v02.01.03

 - Modified sampling code to only cast rays that actually fall within a
   triangle boundary. 
 - Improved super-sampling code to sum contributions from multiple
   triangles per texel 
 - Fixed -v option (which specifies maximum distance for normals from
   the low resolution model) 
 - Added -a and -A options to change cutoff angle that determines if a
   normal is even considered as a potential normal. 
 - Improve tangent space computation to remove singularities.
 - General code cleanup and organization.