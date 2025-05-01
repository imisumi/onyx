#version 460 core

in vec3 worldPos;
out vec4 FragColor;

uniform vec3 cameraPosition;

// Simple grid constants
const vec4 minorLineColor = vec4(0.15, 0.2, 0.25, 0.2);
const vec4 majorLineColor = vec4(0.3, 0.4, 0.45, 0.4);
const vec4 axisColor = vec4(0.0, 0.7, 0.0, 0.5);
const float fadeEndDistance = 200.0;

// Ultra-simple anti-aliased line with fixed width
float getGridLine(float position) {
    float posInCell = fract(position);
    float distToLine = min(posInCell, 1.0 - posInCell);
    
    // Fixed screen-space width (1 pixel)
    float pixelWidth = fwidth(position);
    
    // Simple smoothstep for anti-aliasing
    return 1.0 - smoothstep(0.0, 1.5 * pixelWidth, distToLine);
}

void main()
{
    // Simple distance fade factor
    float distanceFromCamera = distance(worldPos, cameraPosition);
    float fadeFactor = max(0.0, 1.0 - distanceFromCamera / fadeEndDistance);
    
    // Grid coordinates
    vec2 coord = worldPos.xz;
    
    // Calculate line intensities separately to avoid artifacts
    float minorLineX = getGridLine(coord.x);
    float minorLineZ = getGridLine(coord.y);
    
    float majorLineX = getGridLine(coord.x * 0.1); // 10-unit grid
    float majorLineZ = getGridLine(coord.y * 0.1); // 10-unit grid
    
    // Z axis (vertical green line)
    float axisLine = (abs(coord.x) < 0.02) ? 1.0 : 0.0;
    
    // Start with transparent black
    vec4 color = vec4(0.0);
    
    // Layer the lines from minor to major to axis
    if (minorLineX > 0.0 || minorLineZ > 0.0) {
        float lineStrength = max(minorLineX, minorLineZ);
        color = mix(color, minorLineColor, lineStrength * fadeFactor);
    }
    
    if (majorLineX > 0.0 || majorLineZ > 0.0) {
        float lineStrength = max(majorLineX, majorLineZ);
        color = mix(color, majorLineColor, lineStrength * fadeFactor);
    }
    
    if (axisLine > 0.0) {
        color = mix(color, axisColor, axisLine * fadeFactor);
    }
    
    // Discard transparent fragments
    if (color.a < 0.01) {
        discard;
    } else {
        FragColor = color;
    }
}