## IA725 - Computação Gráfica I (Computer Graphics I)

This repository contains all exercises of the subject IA725 (Computer Graphics I) of FEEC - Unicamp by professor Wu Shin Ting on the 2nd semester of 2021. [Course link](https://www.dca.fee.unicamp.br/cursos/IA725/2s2021/).

All codes are based on modified versions of the OpenGL tutorials made by Etay Meiri that can be found on [ogldev.org](ogldev.org). His GitHub repository: https://github.com/emeiri/ogldev .

Techs used:
- Lang: C++
- Libs: FreeGLUT, GLEW, glm (ArcBall calculation)
- API: required OpenGL >= 3
- IDE: Visual Studio 2019
- All codes are being tested on a computer that has a NVIDIA GeForce 610M video card that supports OpenGL up to 4.6 (NVIDIA 388.73 driver).

Exercises:

| Exercise | Description  | Status |
| --- | --- | --- |
| 1 | Theoretical exercises done on Moodle to review mathematical concepts to use in computer graphics. | :heavy_check_mark: |
| 2  | Implement three programs: a table (brown), an icosahedron (yellow) subdivided in order 3, and a Utah teapot (blue), centered on the origin.  | :heavy_check_mark: |
| 3  | Implement a scene of a table on which are the Utah teapot and the icosahedron subdivided in order 4. You can rotate the scene by an arcball.  | :heavy_check_mark:  |
| 4  | Add to the scene of the table with the sphere and the Utah teapot two functionalities: (a) selection of projection type, parallel and perspective, via **glutCreateMenu**; (b) navigation through the scene in a manner analogous to the interactions implemented in [Tutorial 14](https://ogldev.org/www/tutorial14/tutorial14.html) and [Tutorial 15](https://ogldev.org/www/tutorial15/tutorial15.html).  | :heavy_check_mark:  |
| 5  | Design: add clipping functionality by a plane within the view volume (plane repositioning via terminal keys). Peer evaluation of the projects.  | :heavy_check_mark:  |
| 6  | Implement a function to edit the H,S,V components of the icosahedron color in the table scene with the Utah teapot and the icosahedron subdivided up to order 3 by means of the following interactions: H increases the angle and h reduces the angle on the color disk by 10 units (chroma); S increases the white and s reduces the white by 10 units (saturation), and V increases the brightness and v reduces the brightness by 10 units (luminance).  | :heavy_check_mark:  |
| 7  | Add a white light source to the scene of the table with the sphere and the Utah teapot. Apply the *Blinn-Phong* lighting model and three types of shading (*Flat*, *Gouraud* and *Phong*) to "colorize" the three objects in the scene. Add this new feature as an item, containing 3 sub-items to the menu you created earlier.  | :heavy_check_mark: |
| 8  | Theoretical exercises done on Moodle to study about rasterization in computer graphics.  | :heavy_check_mark: |
| 9  | Add to the scene of the table with the sphere and the Utah teapot a shadow of the table in relation to the white light source.  | :x: |
| 10 | Project: Add to the table scene with the Utah sphere and teapot a wooden texture for the table, a bump texture for the 4-order icosahedron, and a well-polished, smooth-surfaced teapot. Peer evaluation of the projects.  | :x: |
| 11 | Theoretical exercises done on Moodle to study about modern graphics hardware in computer graphics. | :heavy_check_mark: |