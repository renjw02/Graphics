#version 330 core

out vec4 FragColor;

void main() {
    // 计算窗口坐标（范围从-1到1）
    vec2 windowCoords = (gl_FragCoord.xy / gl_FragCoord.w) * 2.0 - 1.0;

    // 将窗口坐标进行反相
    vec2 invertedWindowCoords = 1.0 - abs(windowCoords);

    // 将窗口坐标重新映射回[0,1]范围
    vec2 finalCoords = (invertedWindowCoords + 1.0) * 0.5;

    // 输出颜色
    FragColor = vec4(finalCoords, 0.0, 1.0);
}
