#version 460 core
in vec4 lineColor;
out vec4 FragColor;

void main() {
	FragColor = lineColor;
	
	// Optional: make grid fade with distance
	float dist = length(gl_FragCoord.xy - vec2(2560 / 2, 1440 / 2));
	float alpha = 1.0 - (dist / 1000.0);
	FragColor.a *= clamp(alpha, 0.0, 1.0);
}