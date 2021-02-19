/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <iostream>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

//! [1]
class OpenGLWindow : public QWindow, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

protected:

	void vglAttachShaderSource(GLuint prog, GLenum type, const char * source)
	{
		GLuint sh;

		sh = glCreateShader(type);
		glShaderSource(sh, 1, &source, NULL);
		glCompileShader(sh);
		char buffer[4096];
		glGetShaderInfoLog(sh, sizeof(buffer), NULL, buffer);
		glAttachShader(prog, sh);
		glDeleteShader(sh);
	}

	static const GLchar*
		ReadShader(const char* filename)
	{
#ifdef WIN32
		FILE* infile;
		fopen_s(&infile, filename, "rb");
#else
		FILE* infile = fopen(filename, "rb");
#endif // WIN32

		if (!infile) {
#ifdef _DEBUG
			std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
			return NULL;
		}

		fseek(infile, 0, SEEK_END);
		int len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		GLchar* source = new GLchar[len + 1];

		fread(source, 1, len, infile);
		fclose(infile);

		source[len] = 0;

		return const_cast<const GLchar*>(source);
	}

	//----------------------------------------------------------------------------

	GLuint
		LoadShaders(ShaderInfo* shaders)
	{
		if (shaders == NULL) { return 0; }

		GLuint program = glCreateProgram();

		ShaderInfo* entry = shaders;
		while (entry->type != GL_NONE) {
			GLuint shader = glCreateShader(entry->type);

			entry->shader = shader;

			const GLchar* source = ReadShader(entry->filename);
			if (source == NULL) {
				for (entry = shaders; entry->type != GL_NONE; ++entry) {
					glDeleteShader(entry->shader);
					entry->shader = 0;
				}

				return 0;
			}

			glShaderSource(shader, 1, &source, NULL);
			delete[] source;

			glCompileShader(shader);

			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
#ifdef _DEBUG
				GLsizei len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				GLchar* log = new GLchar[len + 1];
				glGetShaderInfoLog(shader, len, &len, log);
				std::cerr << "Shader compilation failed:" << "(" << entry->filename << ") " << log << std::endl;
				delete[] log;
#endif /* DEBUG */

				return 0;
			}

			glAttachShader(program, shader);

			++entry;
		}

#ifdef GL_VERSION_4_1
		// 		if (GLEW_VERSION_4_1) {
		// 			// glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
		// 		}
#endif /* GL_VERSION_4_1 */

		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		for (entry = shaders; entry->type != GL_NONE; ++entry) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		if (!linked) {
#ifdef _DEBUG
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			std::cerr << "Shader linking failed: " << log << std::endl;
			delete[] log;
#endif /* DEBUG */

			return 0;
		}

		return program;
	}

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};
//! [1]

