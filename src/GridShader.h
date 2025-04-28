#ifndef GRID_SHADER_H
#define GRID_SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class GridShader
{
private:
	GLuint programID;
	GLuint VAO, VBO, EBO;
	GLuint mvpLocation;
	GLuint colorLocation;
	GLuint isMainLineLocation;
	GLuint regularLinesOnlyLocation;
	GLuint xAxisColorLocation;
	GLuint zAxisColorLocation;
	GLuint yAxisColorLocation;
	GLuint playerPositionLocation;
	GLuint maxDistanceLocation;
	GLuint fadeDistanceLocation;

	int gridSize;
	float cellSize;
	glm::vec4 gridColor;
	glm::vec4 mainGridColor;
	glm::vec4 xAxisColor;	  // Color for X axis (typically red)
	glm::vec4 zAxisColor;	  // Color for Z axis (typically blue)
	glm::vec4 yAxisColor;	  // Color for Y axis (typically green)
	float fadeDistance;		  // Distance at which grid begins to fade
	float maxDistance;		  // Distance at which grid is fully transparent
	glm::vec3 playerPosition; // Current player position for infinite grid

	// Store indices for axes
	std::vector<int> _xAxisIndices;
	std::vector<int> _zAxisIndices;
	std::vector<int> _yAxisIndices;

	const char *vertexShaderSource = R"(
		#version 330 core
		layout(location = 0) in vec3 position;
		layout(location = 1) in float isMainLine;
		
		uniform mat4 MVP;
		uniform vec3 playerPosition;
		uniform float maxDistance;
		
		out float v_isMainLine;
		out float v_fadeAlpha;
		out vec3 v_worldPos;  // Pass world position to fragment shader
		
		void main() {
			vec3 adjustedPos = position;
			
			// Move in 10m increments
			float snapX = floor(playerPosition.x / 10.0) * 10.0;
			float snapZ = floor(playerPosition.z / 10.0) * 10.0;
			adjustedPos.x += snapX;
			adjustedPos.z += snapZ;
			
			gl_Position = MVP * vec4(adjustedPos, 1.0);
			v_isMainLine = isMainLine;
			v_worldPos = adjustedPos;  // Pass adjusted world position to fragment shader
		}
	)";

	const char *fragmentShaderSource = R"(
		#version 330 core
		in float v_isMainLine;
		in vec3 v_worldPos;
		out vec4 FragColor;
		
		uniform vec4 gridColor;
		uniform vec4 mainGridColor;
		uniform vec3 playerPosition;
		uniform float maxDistance;
		uniform float fadeDistance;
		
		void main() {
			// Choose base color
			vec4 baseColor;
			if (v_isMainLine > 0.5) {
				baseColor = mainGridColor;
			} else {
				baseColor = gridColor;
			}
			
			// Calculate distance from player
			float distFromPlayer = length(v_worldPos.xz - playerPosition.xz);
			
			// Very gradual fade that starts almost immediately
			float innerRadius = fadeDistance;        // Start fading at fadeDistance
			float outerRadius = maxDistance;         // Full fade by maxDistance
			
			// Use cubic function for even smoother falloff
			float t = clamp((distFromPlayer - innerRadius) / (outerRadius - innerRadius), 0.0, 1.0);
			float fadeAlpha = 1.0 - (t * t * t); // Cubic falloff for very smooth transition
			
			// Apply fade to alpha
			FragColor = vec4(baseColor.rgb, baseColor.a * fadeAlpha);
		}
	)";

	GLuint compileShader(GLenum type, const char *source)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		// Check for compilation errors
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
		}

		return shader;
	}

	void createShaderProgram()
	{
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		// Check for linking errors
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Get uniform locations
		mvpLocation = glGetUniformLocation(programID, "MVP");
		colorLocation = glGetUniformLocation(programID, "gridColor");
		isMainLineLocation = glGetUniformLocation(programID, "mainGridColor");
		regularLinesOnlyLocation = glGetUniformLocation(programID, "regularLinesOnly");
		xAxisColorLocation = glGetUniformLocation(programID, "xAxisColor");
		zAxisColorLocation = glGetUniformLocation(programID, "zAxisColor");
		yAxisColorLocation = glGetUniformLocation(programID, "yAxisColor");
		playerPositionLocation = glGetUniformLocation(programID, "playerPosition");
		maxDistanceLocation = glGetUniformLocation(programID, "maxDistance");
		fadeDistanceLocation = glGetUniformLocation(programID, "fadeDistance");
	}

	void setupGrid()
	{
		std::vector<float> vertices;
		std::vector<float> isMainLine;
		std::vector<int> xAxisIndices;
		std::vector<int> zAxisIndices;
		std::vector<int> yAxisIndices;

		// Create horizontal lines (along X axis, varying Z)
		for (int i = -gridSize; i <= gridSize; i++)
		{
			bool isMain = (i % 10 == 0);
			bool isZAxis = (i == 0); // This is the Z axis (X=0)

			// Track Z axis indices
			if (isZAxis)
			{
				zAxisIndices.push_back(vertices.size() / 3);
				zAxisIndices.push_back(vertices.size() / 3 + 1);
			}

			// Line start point
			vertices.push_back(-gridSize * cellSize); // x
			vertices.push_back(0.0f);				  // y
			vertices.push_back(i * cellSize);		  // z
			isMainLine.push_back(isMain ? 1.0f : 0.0f);

			// Line end point
			vertices.push_back(gridSize * cellSize); // x
			vertices.push_back(0.0f);				 // y
			vertices.push_back(i * cellSize);		 // z
			isMainLine.push_back(isMain ? 1.0f : 0.0f);
		}

		// Create vertical lines (along Z axis, varying X)
		for (int i = -gridSize; i <= gridSize; i++)
		{
			bool isMain = (i % 10 == 0);
			bool isXAxis = (i == 0); // This is the X axis (Z=0)

			// Track X axis indices
			if (isXAxis)
			{
				xAxisIndices.push_back(vertices.size() / 3);
				xAxisIndices.push_back(vertices.size() / 3 + 1);
			}

			// Line start point
			vertices.push_back(i * cellSize);		  // x
			vertices.push_back(0.0f);				  // y
			vertices.push_back(-gridSize * cellSize); // z
			isMainLine.push_back(isMain ? 1.0f : 0.0f);

			// Line end point
			vertices.push_back(i * cellSize);		 // x
			vertices.push_back(0.0f);				 // y
			vertices.push_back(gridSize * cellSize); // z
			isMainLine.push_back(isMain ? 1.0f : 0.0f);
		}

		// Add Y-axis (a vertical line at the origin)
		int yAxisStartIndex = vertices.size() / 3;

		// Y-axis line from origin up to gridSize
		vertices.push_back(0.0f);	// x
		vertices.push_back(0.0f);	// y - start at ground level
		vertices.push_back(0.0f);	// z
		isMainLine.push_back(1.0f); // Treat as main line

		vertices.push_back(0.0f);				 // x
		vertices.push_back(gridSize * cellSize); // y - extend upward
		vertices.push_back(0.0f);				 // z
		isMainLine.push_back(1.0f);				 // Treat as main line

		// Add Y-axis indices
		yAxisIndices.push_back(yAxisStartIndex);
		yAxisIndices.push_back(yAxisStartIndex + 1);

		// Store axis indices for rendering
		_xAxisIndices = xAxisIndices;
		_zAxisIndices = zAxisIndices;
		_yAxisIndices = yAxisIndices;

		// Create and setup VAO and VBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		GLuint isMainVBO;
		glGenBuffers(1, &isMainVBO);

		glBindVertexArray(VAO);

		// Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// isMainLine attribute
		glBindBuffer(GL_ARRAY_BUFFER, isMainVBO);
		glBufferData(GL_ARRAY_BUFFER, isMainLine.size() * sizeof(float), isMainLine.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);

		// Set up Element Buffer Object for axis lines
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// Combine all indices
		std::vector<GLuint> allIndices;
		for (int idx : xAxisIndices)
			allIndices.push_back(idx);
		for (int idx : zAxisIndices)
			allIndices.push_back(idx);
		for (int idx : yAxisIndices)
			allIndices.push_back(idx);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(GLuint), allIndices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

public:
	GridShader(int gridSize = 100, float cellSize = 1.0f,
			   glm::vec4 gridColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
			   glm::vec4 mainGridColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
			   glm::vec4 xAxisColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			   glm::vec4 zAxisColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			   glm::vec4 yAxisColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			   float fadeDistance = 100.0f,
			   float maxDistance = 150.0f)
		: gridSize(gridSize), cellSize(cellSize), gridColor(gridColor),
		  mainGridColor(mainGridColor), xAxisColor(xAxisColor),
		  zAxisColor(zAxisColor), yAxisColor(yAxisColor),
		  fadeDistance(fadeDistance), maxDistance(maxDistance),
		  playerPosition(glm::vec3(0.0f))
	{
		createShaderProgram();
		setupGrid();
	}

	~GridShader()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(programID);
	}

	void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &playerPos = glm::vec3(0.0f))
	{
		// Update player position for infinite grid
		playerPosition = playerPos;

		glm::mat4 model = glm::mat4(1.0f); // Identity matrix
		glm::mat4 mvp = projection * view * model;

		glUseProgram(programID);
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		// Pass player position and max distance for fading
		glUniform3fv(playerPositionLocation, 1, glm::value_ptr(playerPosition));
		glUniform1f(maxDistanceLocation, maxDistance);

		glUniform1f(fadeDistanceLocation, fadeDistance);

		// Enable blending for fade effect
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(VAO);

		// First render pass: Draw regular grid lines (thin, grey)
		glUniform4fv(colorLocation, 1, glm::value_ptr(gridColor));
		glUniform4fv(isMainLineLocation, 1, glm::value_ptr(mainGridColor));
		glLineWidth(1.0f);

		// Draw only regular grid lines
		glUniform1i(regularLinesOnlyLocation, 1);
		glDrawArrays(GL_LINES, 0, (gridSize * 2 + 1) * 4);

		// Second render pass: Draw main grid lines (thicker, brighter)
		glLineWidth(1.0f);

		// Draw only main grid lines
		glUniform1i(regularLinesOnlyLocation, 0);
		glDrawArrays(GL_LINES, 0, (gridSize * 2 + 1) * 4);

		// Disable the shader uniforms related to the grid
		// and set up a separate rendering approach for the axes
		glUseProgram(0); // Stop using the grid shader

		// Create a simpler shader just for the axes
		static GLuint axisShaderProgram = 0;
		if (axisShaderProgram == 0)
		{
			// Create a simple shader for the axes
			const char *axisVertexShader = R"(
                #version 330 core
                layout(location = 0) in vec3 position;
                uniform mat4 MVP;
                void main() {
                    gl_Position = MVP * vec4(position, 1.0);
                }
            )";

			const char *axisFragmentShader = R"(
                #version 330 core
                uniform vec4 axisColor;
                out vec4 FragColor;
                void main() {
                    FragColor = axisColor;
                }
            )";

			// Compile and link the axis shader
			GLuint axisVS = compileShader(GL_VERTEX_SHADER, axisVertexShader);
			GLuint axisFS = compileShader(GL_FRAGMENT_SHADER, axisFragmentShader);
			axisShaderProgram = glCreateProgram();
			glAttachShader(axisShaderProgram, axisVS);
			glAttachShader(axisShaderProgram, axisFS);
			glLinkProgram(axisShaderProgram);
			glDeleteShader(axisVS);
			glDeleteShader(axisFS);
		}

		// Use the axis shader
		glUseProgram(axisShaderProgram);
		GLint axisMvpLoc = glGetUniformLocation(axisShaderProgram, "MVP");
		GLint axisColorLoc = glGetUniformLocation(axisShaderProgram, "axisColor");
		glUniformMatrix4fv(axisMvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glLineWidth(2.0f); // Thicker lines for axes

		// Create temporary VAO and VBO for X axis
		GLuint axisVAO, axisVBO;
		glGenVertexArrays(1, &axisVAO);
		glGenBuffers(1, &axisVBO);

		// Create X axis line - from origin to +X direction
		float xAxisVertices[] = {
			0.0f, 0.0f, 0.0f,				// Origin
			gridSize * cellSize, 0.0f, 0.0f // +X direction
		};

		// Set up X axis VAO
		glBindVertexArray(axisVAO);
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(xAxisVertices), xAxisVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		// Draw X axis in red
		glUniform4fv(axisColorLoc, 1, glm::value_ptr(xAxisColor));
		glDrawArrays(GL_LINES, 0, 2);

		// Y axis vertices
		float yAxisVertices[] = {
			0.0f, 0.0f, 0.0f,				// Origin
			0.0f, gridSize * cellSize, 0.0f // +Y direction
		};

		// Update buffer with Y axis data
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(yAxisVertices), yAxisVertices, GL_STATIC_DRAW);

		// Draw Y axis in green
		glUniform4fv(axisColorLoc, 1, glm::value_ptr(yAxisColor));
		glDrawArrays(GL_LINES, 0, 2);

		// Z axis vertices
		float zAxisVertices[] = {
			0.0f, 0.0f, 0.0f,				// Origin
			0.0f, 0.0f, gridSize * cellSize // +Z direction
		};

		// Update buffer with Z axis data
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(zAxisVertices), zAxisVertices, GL_STATIC_DRAW);

		// Draw Z axis in blue
		glUniform4fv(axisColorLoc, 1, glm::value_ptr(zAxisColor));
		glDrawArrays(GL_LINES, 0, 2);

		// Reset state and clean up resources
		glBindVertexArray(0);
		glUseProgram(0);
		glLineWidth(1.0f);
		glDisable(GL_BLEND);

		// Clean up temporary resources
		glDeleteVertexArrays(1, &axisVAO);
		glDeleteBuffers(1, &axisVBO);
	}
};

#endif // GRID_SHADER_H