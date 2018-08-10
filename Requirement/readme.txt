Create a 3D application (run on window)
1: Load and render 2 obj files: ancient_apparition_flat.obj and ancient_apparition_smooth.obj, they use same texture: ancient_apparition_base_color.tga
2: Add basic lighting
3: Add camera (MVP)
4: Reduce vertices of smoothly-shaded model by using indicates. (plus point)

Note:

Load OBJ: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

Smooth shading: two adjacent triangles must share common vertex normals. The reason is interpolation. The normal from one vertex is smoothly interpolated to the normal on the second vertex. You need to ensure that the normal of triangle at a particular vertex matches the normal of all adjacent triangles at that same vertex. If you think of a smooth sphere, each vertex on the surface (which is used by multiple triangles) has a single normal.

Flat shading: the normal for each vertex on every triangle must be the same so that when interpolated the value is the same across the whole surface. This means that if a single vertex is shared by multiple triangles then the vertex must be duplicated for each triangle in order to have an independent normal for each triangle.\

Overall this means that a smoothly-shaded model can share vertices in ways that a flat-shaded model cannot.