#include "wavefront_obj.hh"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../math/math.h"
#include "../core/vertex.hh"

#include "../utils/color.h"
#include "../utils/consc.h"

Model* wfobj_loadColored(const char* _objPath, enum IndicesOrder _order) {
	FILE* file = fopen(_objPath, "r");
	if (file == nullptr) {
		printf(AC_RED "file is NULL: %s\n" AC_RESET, _objPath);
		return nullptr;
	}

	Model* model { nullptr };

	char c;
	int vert { 0 };
	int tris { 0 };

	bool detectOrder { _order == IndicesOrder::Auto ? true : false };
	Vector3 normal V3_ZERO;
	
	bool flipX { false };
	{
		char buf[10];
		char* line = fgets(buf, 10, file);
		if (line == nullptr) {
			fclose(file);
			return nullptr;
		}

		if (strcmp(line, "# Blender") == 0) flipX = true;
	}

	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			if ((c = getc(file)) == ' ') {
				vert++;
				while ((c = getc(file)) != '\n' && c != EOF) ;
			}
			else if (detectOrder && c == 'n') {
				int n = fscanf(file, "%f %f %f", &normal.X, &normal.Y, &normal.Z);

				if (n != 3) {
					printf(AC_RED "Failed to match vertex normal data." AC_RESET);

					fclose(file);
					return nullptr;
				}
				detectOrder = false;
			}
		}
		else if (c == 'f') {
			if (getc(file) == ' ') {
				int vCount { 1 };
				while ((c = getc(file)) != '\n' && c != EOF) {
					if (c == ' ') vCount++;
				}
				if (vCount == 4) tris += 2;
				else if (vCount == 3) tris++;
			}
		}
	}
	
	if (_order == IndicesOrder::Auto) {
		if (normal == V3_ZERO) {
			printf(AC_RED "Failed to retrive vertex normal." AC_RESET);

			fclose(file);
			return nullptr;
		}
	}
	
	Vertex_Colored* vertices { (Vertex_Colored*)malloc(sizeof(struct Vertex_Colored) * vert) };
	uint16_t* indices { (uint16_t*)malloc(sizeof(uint16_t) * tris * 3) };
	
	int vIndex { 0 };
	int iIndex { 0 };
	
	rewind(file);
	
	while ((c = getc(file)) != EOF) {
		if (c == 'v') {
			if (getc(file) == ' ') {
				float x, y, z;
				float r, g, b;
				int n = fscanf(file, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				
				if (n == 3) {
					vertices[vIndex] = Vertex_Colored(!flipX ? x : -x, y, z, 0xff7f00ff);
				}
				else if (n == 6) {
					vertices[vIndex] = Vertex_Colored(!flipX ? x : -x, y, z, rgbToHex(r, g, b));
				}
				else {
					printf(AC_RED "Failed to match vertex data." AC_RESET);
					goto end;
				}
				vIndex++;
			}
		}
		else if (c == 'f') {
			if (getc(file) == ' ') {
				int
					v1 { -1 }, vt1 { -1 }, vn1 { -1 },
					v2 { -1 }, vt2 { -1 }, vn2 { -1 },
					v3 { -1 }, vt3 { -1 }, vn3 { -1 },
					v4 { -1 }, vt4 { -1 }, vn4 { -1 };

				int n = fscanf(file,
					"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
					&v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3, &v4, &vt4, &vn4);
				
				// we store indexes 0 based
				v1--; vt1--; vn1--;
				v2--; vt2--; vn2--;
				v3--; vt3--; vn3--;
				v4--; vt4--; vn4--;

				if (_order == IndicesOrder::Auto) {
					Vector3 a { V3_NEW(vertices[v1].x, vertices[v1].y, vertices[v1].z) };
					Vector3 b { V3_NEW(vertices[v2].x, vertices[v2].y, vertices[v2].z) };
					Vector3 c { V3_NEW(vertices[v3].x, vertices[v3].y, vertices[v3].z) };
					
					// calculate normal clockwise
					Vector3 normCalculated { V3_NORM(V3_CROSS(b - a, c - a)) };
					
					// hopefully that will be enough.
					// EDIT: changed to 0.95 from 0.99
					// theoretically dot > 0.0f would be enough, but it is strange that even 0.975 happened, maybe should investigate this
					_order = V3_DOT(normal, normCalculated) >= 0.95f ? IndicesOrder::CW : IndicesOrder::CCW;
					
					/*printf(AC_YELLOW "determine indices order for \"%s\"\n" AC_RESET
						"           normal: %f %f %f\nnormal calculated: %f %f %f\nDOT: %f\n",
						_objPath, normal.X, normal.Y, normal.Z, normCalculated.X, normCalculated.Y, normCalculated.Z, V3_DOT(normal, normCalculated));*/
				}

				if (n == 12) {
					// Blender arranges indices clockwise, while bgfx processes counterclockwise
					if (_order == IndicesOrder::CW) {
						indices[iIndex++] = v3;
						indices[iIndex++] = v2;
						indices[iIndex++] = v1;
						
						indices[iIndex++] = v4;
						indices[iIndex++] = v3;
						indices[iIndex++] = v1;
					}
					else {
						indices[iIndex++] = v1;
						indices[iIndex++] = v2;
						indices[iIndex++] = v3;
						
						indices[iIndex++] = v1;
						indices[iIndex++] = v3;
						indices[iIndex++] = v4;
					}
					
				}
				else if (n == 9) {
					if (_order == IndicesOrder::CW) {
						indices[iIndex++] = v3;
						indices[iIndex++] = v2;
						indices[iIndex++] = v1;
					}
					else {
						indices[iIndex++] = v1;
						indices[iIndex++] = v2;
						indices[iIndex++] = v3;
					}
				}
				else {
					printf(AC_RED "Failed to match tri/quad data." AC_RESET);
					goto end;
				}
			}
		}
	}
	
	/*puts(AC_YELLOW "VERTICES" AC_RESET);
	for (int i = 0; i < vert; i++) printf("%d:\t%f %f %f\n", i + 1, vertices[i].x, vertices[i].y, vertices[i].z);
	puts(AC_YELLOW "INDICES" AC_RESET);
	for (int i = 1, j = 2; j < tris * 3; i++, j += 3) printf("%d:\t%d %d %d\n", i, indices[j - 2], indices[j - 1], indices[j]);*/
	
	model = ModelManager::create(vertices, vert, indices, tris * 3, Vertex_Colored::layout);

end:
	free(vertices);
	free(indices);

	fclose(file);

	return model;
}
