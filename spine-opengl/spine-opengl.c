/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated July 28, 2023. Replaces all prior versions.
 *
 * Copyright (c) 2013-2023, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software or
 * otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE
 * SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "spine-opengl.h"
#include <spine/spine.h>
#include <spine/extension.h>
#include <stddef.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

_SP_ARRAY_IMPLEMENT_TYPE_NO_CONTAINS(spVertexArray, Vertex)

GLBlendMode blend_normal = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
GLBlendMode blend_additive = {GL_SRC_ALPHA, GL_ONE};
GLBlendMode blend_multiply = {GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA};
GLBlendMode blend_screen = {GL_ONE, GL_ONE_MINUS_SRC_COLOR};

GLBlendMode blend_normalPma = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
GLBlendMode blend_additivePma = {GL_ONE, GL_ONE};
GLBlendMode blend_multiplyPma = {GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA};
GLBlendMode blend_screenPma = {GL_ONE, GL_ONE_MINUS_SRC_COLOR};

spSkeletonDrawable *spSkeletonDrawable_create(spSkeletonData *skeletonData, spAnimationStateData *animationStateData)
{
	spBone_setYDown(-1);
	spSkeletonDrawable *self = NEW(spSkeletonDrawable);
	self->skeleton = spSkeleton_create(skeletonData);
	self->animationState = spAnimationState_create(animationStateData);
	self->usePremultipliedAlpha = 0;
	self->indices = spUnsignedShortArray_create(12);
	self->vertices = spVertexArray_create(12);
	self->worldVertices = spFloatArray_create(12);
	self->clipper = spSkeletonClipping_create();
	self->blendMode = blend_normal;

	self->program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vss = ("#version 330 core\n"
					   "layout(location = 0) in vec2 position;\n"
					   "layout(location = 1) in vec4 color;\n"
					   "layout(location = 2) in vec2 texCoord;\n"
					   "uniform vec2 ourSize;\n"
					   "out vec4 v_color;\n"
					   "out vec2 v_texCoord;\n"
					   "void main() {\n"
					//    "	gl_Position = vec4(position.x / ourSize.x * 2.0 - 1.0, 1.0 - position.y / ourSize.y * 2.0, 0.0, 1.0);\n"
					 "  gl_Position = vec4((position.x-ourSize.x/2.0)/ourSize.x, (ourSize.y/2.0-position.y)/ourSize.y, 0.0, 1.0);"
					   "	v_color = color;\n"
					   "	v_texCoord = texCoord;\n"
					   "}");
	glShaderSource(vs, 1, &vss, NULL);
	glCompileShader(vs);
	GLint success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		fprintf(stderr, "[%s SHADER COMPILE ERROR]\n%s\n", "VERTEX", info_log);
		return 0;
	}
	const char *fss = ("#version 330 core\n"
					   "in vec4 v_color;\n"
					   "in vec2 v_texCoord;\n"
					   "out vec4 FragColor;\n"
					   "uniform sampler2D tex;\n"
					   "void main() {\n"
					   "	FragColor = texture(tex, v_texCoord) * v_color;\n"
					   "}");
	glShaderSource(fs, 1, &fss, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetShaderInfoLog(fs, 512, NULL, info_log);
		fprintf(stderr, "[%s SHADER COMPILE ERROR]\n%s\n", "FRAGMENT", info_log);
		return 0;
	}
	glAttachShader(self->program, vs);
	glAttachShader(self->program, fs);
	glLinkProgram(self->program);

	glGetProgramiv(self->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetProgramInfoLog(self->program, 512, NULL, info_log);
		fprintf(stderr, "[PROGRAM LINK ERROR]\n%s\n", info_log);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(self->program);

	glGenBuffers(1, &self->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));

	return self;
}

void spSkeletonDrawable_dispose(spSkeletonDrawable *self)
{
	spSkeleton_dispose(self->skeleton);
	spAnimationState_dispose(self->animationState);
	spUnsignedShortArray_dispose(self->indices);
	spVertexArray_dispose(self->vertices);
	spFloatArray_dispose(self->worldVertices);
	spSkeletonClipping_dispose(self->clipper);
	FREE(self);
}

void spSkeletonDrawable_update(spSkeletonDrawable *self, float delta, spPhysics physics)
{
	spAnimationState_update(self->animationState, delta);
	spAnimationState_apply(self->animationState, self->skeleton);
	spSkeleton_update(self->skeleton, delta);
	spSkeleton_updateWorldTransform(self->skeleton, physics);
}

void spSkeletonDrawable_draw(spSkeletonDrawable *self, struct SDL_Renderer *renderer)
{
	static unsigned short quadIndices[] = {0, 1, 2, 2, 3, 0};
	spSkeleton *skeleton = self->skeleton;
	spSkeletonClipping *clipper = self->clipper;

	GLTexture *glTex = NULL;
	Vertex vertex;
	for (int i = 0; i < skeleton->slotsCount; ++i)
	{
		spSlot *slot = skeleton->drawOrder[i];
		spAttachment *attachment = slot->attachment;
		if (!attachment)
		{
			spSkeletonClipping_clipEnd(clipper, slot);
			continue;
		}

		// Early out if the slot color is 0 or the bone is not active
		if (slot->color.a == 0 || !slot->bone->active)
		{
			spSkeletonClipping_clipEnd(clipper, slot);
			continue;
		}

		spFloatArray *vertices = self->worldVertices;
		int verticesCount = 0;
		float *uvs = NULL;
		unsigned short *indices;
		int indicesCount = 0;
		spColor *attachmentColor = NULL;

		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment *region = (spRegionAttachment *)attachment;
			attachmentColor = &region->color;

			// Early out if the slot color is 0
			if (attachmentColor->a == 0)
			{
				spSkeletonClipping_clipEnd(clipper, slot);
				continue;
			}

			spFloatArray_setSize(vertices, 8);
			spRegionAttachment_computeWorldVertices(region, slot, vertices->items, 0, 2);
			verticesCount = 4;
			uvs = region->uvs;
			indices = quadIndices;
			indicesCount = 6;
			glTex = (GLTexture *)((spAtlasRegion *)region->rendererObject)->page->rendererObject;
		}
		else if (attachment->type == SP_ATTACHMENT_MESH)
		{
			spMeshAttachment *mesh = (spMeshAttachment *)attachment;
			attachmentColor = &mesh->color;

			// Early out if the slot color is 0
			if (attachmentColor->a == 0)
			{
				spSkeletonClipping_clipEnd(clipper, slot);
				continue;
			}

			spFloatArray_setSize(vertices, mesh->super.worldVerticesLength);
			spVertexAttachment_computeWorldVertices(SUPER(mesh), slot, 0, mesh->super.worldVerticesLength, vertices->items, 0, 2);
			verticesCount = mesh->super.worldVerticesLength >> 1;
			uvs = mesh->uvs;
			indices = mesh->triangles;
			indicesCount = mesh->trianglesCount;
			glTex = (GLTexture *)((spAtlasRegion *)mesh->rendererObject)->page->rendererObject;
		}
		else if (attachment->type == SP_ATTACHMENT_CLIPPING)
		{
			spClippingAttachment *clip = (spClippingAttachment *)slot->attachment;
			spSkeletonClipping_clipStart(clipper, slot, clip);
			continue;
		}
		else
			continue;

		float r = (skeleton->color.r * slot->color.r * attachmentColor->r);
		float g = (skeleton->color.g * slot->color.g * attachmentColor->g);
		float b = (skeleton->color.b * slot->color.b * attachmentColor->b);
		float a = (skeleton->color.a * slot->color.a * attachmentColor->a);
		vertex.r = r;
		vertex.g = g;
		vertex.b = b;
		vertex.a = a;

		if (spSkeletonClipping_isClipping(clipper))
		{
			spSkeletonClipping_clipTriangles(clipper, vertices->items, verticesCount << 1, indices, indicesCount, uvs, 2);
			vertices = clipper->clippedVertices;
			verticesCount = clipper->clippedVertices->size >> 1;
			uvs = clipper->clippedUVs->items;
			indices = clipper->clippedTriangles->items;
			indicesCount = clipper->clippedTriangles->size;
		}
		spVertexArray_clear(self->vertices);
		for (int ii = 0; ii < verticesCount << 1; ii += 2)
		{
			vertex.x = vertices->items[ii];
			vertex.y = vertices->items[ii + 1];
			vertex.u = uvs[ii];
			vertex.v = uvs[ii + 1];
			spVertexArray_add(self->vertices, vertex);
		}

		if (!self->usePremultipliedAlpha)
		{
			switch (slot->data->blendMode)
			{
			case SP_BLEND_MODE_NORMAL:
				self->blendMode = blend_normal;
				break;
			case SP_BLEND_MODE_MULTIPLY:
				self->blendMode = blend_additive;
				break;
			case SP_BLEND_MODE_ADDITIVE:
				self->blendMode = blend_multiply;
				break;
			case SP_BLEND_MODE_SCREEN:
				self->blendMode = blend_screen;
				break;
			}
		}
		else
		{
			switch (slot->data->blendMode)
			{
			case SP_BLEND_MODE_NORMAL:
				self->blendMode = blend_normalPma;
				break;
			case SP_BLEND_MODE_MULTIPLY:
				self->blendMode = blend_multiplyPma;
				break;
			case SP_BLEND_MODE_ADDITIVE:
				self->blendMode = blend_additivePma;
				break;
			case SP_BLEND_MODE_SCREEN:
				self->blendMode = blend_screenPma;
				break;
			}
		}

		glUseProgram(self->program);

		GLuint sizeLoc = glGetUniformLocation(self->program, "ourSize");
		glUniform2f(sizeLoc, (GLfloat)self->canvasWidth, (GLfloat)self->canvasHeight);

		glEnable(GL_BLEND);
		glBlendFunc(self->blendMode.src, self->blendMode.dst);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glTex->texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTex->uWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTex->vWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTex->minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTex->magFilter);

		glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), self->vertices->items, GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, indices);

		spSkeletonClipping_clipEnd(clipper, slot);
	}

	spSkeletonClipping_clipEnd2(clipper);
}

static unsigned int cvtTextureFilter(spAtlasFilter filter)
{
	switch (filter)
	{
	case SP_ATLAS_NEAREST:
		return GL_NEAREST;
	case SP_ATLAS_LINEAR:
		return GL_LINEAR;
	case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case SP_ATLAS_MIPMAP_LINEAR_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case SP_ATLAS_MIPMAP_LINEAR_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		return GL_LINEAR;
	}

	return GL_LINEAR;
}

static unsigned int cvtTextureWrap(spAtlasWrap wrap)
{
	switch (wrap)
	{
	case SP_ATLAS_MIRROREDREPEAT:
		return GL_MIRRORED_REPEAT;
	case SP_ATLAS_CLAMPTOEDGE:
		return GL_CLAMP_TO_EDGE;
	case SP_ATLAS_REPEAT:
		return GL_REPEAT;
	}

	return GL_REPEAT;
}

void _spAtlasPage_createTexture(spAtlasPage *self, const char *path)
{
	int width, height, components;

	stbi_set_unpremultiply_on_load(1);
	stbi_convert_iphone_png_to_rgb(1);

	stbi_uc *imageData = stbi_load(path, &width, &height, &components, 4);
	if (!imageData)
		return;

	GLTexture *gltex = (GLTexture *)malloc(sizeof(GLTexture));

	gltex->minFilter = cvtTextureFilter(self->minFilter);
	gltex->magFilter = cvtTextureFilter(self->magFilter);
	gltex->uWrap = cvtTextureWrap(self->uWrap);
	gltex->vWrap = cvtTextureWrap(self->vWrap);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	gltex->texId = texture;

	stbi_image_free(imageData);
	self->rendererObject = (void *)gltex;
	return;
}

void _spAtlasPage_disposeTexture(spAtlasPage *self)
{
	GLuint textureId = (GLuint)(size_t)self->rendererObject;
	glDeleteTextures(1, &textureId);
}

char *_spUtil_readFile(const char *path, int *length)
{
	return _spReadFile(path, length);
}
