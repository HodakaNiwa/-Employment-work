xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 -13.37568;-0.01541;0.00000;,
 -10.82117;-0.01541;-7.86203;,
 -10.82115;-0.01541;7.86203;,
 -4.13333;-0.01541;-12.72104;,
 -4.13331;-0.01541;12.72104;,
 4.13331;-0.01541;-12.72104;,
 4.13331;-0.01541;12.72104;,
 10.82115;-0.01541;-7.86203;,
 10.82115;-0.01541;7.86203;,
 13.37568;-0.01541;-0.00000;,
 10.82115;-0.01541;-7.86203;,
 4.13331;-0.01541;-12.72104;,
 0.00000;34.88355;0.00000;,
 -4.13333;-0.01541;-12.72104;,
 -10.82117;-0.01541;-7.86203;,
 -13.37568;-0.01541;0.00000;,
 0.00000;34.88355;0.00000;,
 13.37568;-0.01541;-0.00000;;
 
 18;
 3;0,1,2;,
 3;1,3,2;,
 3;2,3,4;,
 3;3,5,4;,
 3;4,5,6;,
 3;5,7,6;,
 3;6,7,8;,
 3;7,9,8;,
 3;10,11,12;,
 3;11,13,12;,
 3;13,14,12;,
 3;14,15,12;,
 3;0,2,16;,
 3;2,4,16;,
 3;4,6,16;,
 3;6,8,16;,
 3;8,9,16;,
 3;17,10,12;;
 
 MeshMaterialList {
  2;
  18;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.047200;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/MODEL/TEXTURE/enemy000.jpg";
   }
  }
 }
 MeshNormals {
  15;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.385421;-0.922741;,
  0.755433;0.357883;-0.548854;,
  0.288550;0.357884;-0.888064;,
  -0.288550;0.357884;-0.888064;,
  -0.755433;0.357884;-0.548853;,
  -0.933766;0.357883;0.000001;,
  -0.877579;0.385421;0.285143;,
  -0.755433;0.357883;0.548854;,
  -0.288550;0.357884;0.888064;,
  0.288550;0.357884;0.888064;,
  0.542374;0.385421;0.746513;,
  0.755433;0.357883;0.548854;,
  0.933766;0.357883;0.000000;,
  0.893546;0.342468;-0.290331;;
  18;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;2,3,1;,
  3;3,4,1;,
  3;4,5,1;,
  3;5,6,7;,
  3;6,8,7;,
  3;8,9,7;,
  3;9,10,11;,
  3;10,12,11;,
  3;12,13,11;,
  3;13,2,14;;
 }
 MeshTextureCoords {
  18;
  0.554930;0.724800;,
  0.593620;0.724800;,
  0.593620;0.724800;,
  0.694910;0.724800;,
  0.694910;0.724800;,
  0.820120;0.724800;,
  0.820120;0.724800;,
  0.921410;0.724800;,
  0.921410;0.724800;,
  0.960100;0.724800;,
  0.435450;0.719240;,
  0.334160;0.719240;,
  0.271560;0.314080;,
  0.208950;0.719240;,
  0.107660;0.719240;,
  0.068970;0.719240;,
  0.757520;0.319630;,
  0.474140;0.719240;;
 }
}
