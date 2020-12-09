使用sdl2建立窗口进行交互
#Z-Buffer算法
遍历所有三角形
    将每个三角形的三个顶点坐标通过model view projection viewport 转换到屏幕坐标系
    如果通过测试，更新层次z-buffer表并且进行光栅化;否则不必光栅化
    