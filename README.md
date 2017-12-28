#DrawText 使用说明

DrawText 是利用freetyp2库的渲染功能，实现将文字转换为图片的功能。  
---
## 使用方法：
drawtext <Text file> <Bitmap file> -font <path of ttf> [-size <16>] [-multiline <0>] [-align <4>] [-linespace <0>] [-width <512>] [-height <64>] [-color <#ff0000>] [-backcolor <#000000>]  
---
## 参数说明：
### 必选参数
    **-i <Text path>**：文本文件路径，文本文件为UTF-8无BOM格式编码  
    **-o <Bitmap path>**：生成的图片文件路径，图片格式为24bit BMP格式  
    **-font <path of ttf>**: 字体文件路径  

### 可选参数
    [-size <16>] : 字体大小，默认为16像素高  
    [-multiline <0>] ：多行或单行，0为单行，1为多行  
    [-align <4>] ：对齐方式，0-8，对应左上到右下，4为居中  
    [-linespace <0>] ：行距，多行文本时有效，默认为0  
    [-width <512>] ：单页窗口宽度，单行时生成的图片宽度为width的整数倍  
    [-height <64>] ：单页窗口高度，多行时生成图片的高度为height的整数倍  
    [-color <#ff0000>] ：字体颜色  
    [-backcolor <#000000>]：背景颜色  
