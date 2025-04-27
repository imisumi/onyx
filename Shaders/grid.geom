#version 460 core
layout(points) in;
layout(line_strip, max_vertices = 128) out;

uniform mat4 viewProj;
uniform float gridSize = 10.0;
uniform float gridSpacing = 1.0;
uniform vec4 gridColor = vec4(0.5, 0.5, 0.5, 0.6);
uniform vec4 xAxisColor = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 zAxisColor = vec4(0.0, 0.0, 1.0, 1.0);

out vec4 lineColor;

void main() {
	// Calculate how many lines we can draw within our vertex limit
	// We need 2 vertices per line, and we want to ensure we draw the axes
	float adaptiveSpacing = gridSpacing;
	int maxLines = 63; // 126 vertices for lines + 2 for Y axis = 128 max
	
	// If the grid would require too many lines, adjust the spacing
	float totalLines = (gridSize * 2.0) / gridSpacing * 2.0; // X and Z axes
	if (totalLines > float(maxLines)) {
		adaptiveSpacing = (gridSize * 2.0) / (float(maxLines) / 2.0);
	}
	
	// Always draw the axes
	// X axis
	lineColor = xAxisColor;
	gl_Position = viewProj * vec4(-gridSize, 0.0, 0.0, 1.0);
	EmitVertex();
	gl_Position = viewProj * vec4(gridSize, 0.0, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
	// Z axis
	lineColor = zAxisColor;
	gl_Position = viewProj * vec4(0.0, 0.0, -gridSize, 1.0);
	EmitVertex();
	gl_Position = viewProj * vec4(0.0, 0.0, gridSize, 1.0);
	EmitVertex();
	EndPrimitive();
	
	// Y axis
	lineColor = vec4(0.0, 1.0, 0.0, 1.0);  // Green for Y axis
	gl_Position = viewProj * vec4(0.0, 0.0, 0.0, 1.0);
	EmitVertex();
	gl_Position = viewProj * vec4(0.0, gridSize, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
	
	// Draw remaining grid lines
	int linesDrawn = 3; // We've already drawn 3 axes
	
	// Draw grid lines along X axis
	for(float i = adaptiveSpacing; i <= gridSize; i += adaptiveSpacing) {
		if(linesDrawn >= maxLines) break;
		
		// Draw positive X
		lineColor = gridColor;
		gl_Position = viewProj * vec4(i, 0.0, -gridSize, 1.0);
		EmitVertex();
		gl_Position = viewProj * vec4(i, 0.0, gridSize, 1.0);
		EmitVertex();
		EndPrimitive();
		linesDrawn++;
		
		if(linesDrawn >= maxLines) break;
		
		// Draw negative X
		gl_Position = viewProj * vec4(-i, 0.0, -gridSize, 1.0);
		EmitVertex();
		gl_Position = viewProj * vec4(-i, 0.0, gridSize, 1.0);
		EmitVertex();
		EndPrimitive();
		linesDrawn++;
	}
	
	// Draw grid lines along Z axis
	for(float i = adaptiveSpacing; i <= gridSize && linesDrawn < maxLines; i += adaptiveSpacing) {
		if(linesDrawn >= maxLines) break;
		
		// Draw positive Z
		lineColor = gridColor;
		gl_Position = viewProj * vec4(-gridSize, 0.0, i, 1.0);
		EmitVertex();
		gl_Position = viewProj * vec4(gridSize, 0.0, i, 1.0);
		EmitVertex();
		EndPrimitive();
		linesDrawn++;
		
		if(linesDrawn >= maxLines) break;
		
		// Draw negative Z
		gl_Position = viewProj * vec4(-gridSize, 0.0, -i, 1.0);
		EmitVertex();
		gl_Position = viewProj * vec4(gridSize, 0.0, -i, 1.0);
		EmitVertex();
		EndPrimitive();
		linesDrawn++;
	}
}