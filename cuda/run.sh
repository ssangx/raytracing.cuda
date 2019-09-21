nvcc main.cu -o raytracing -gencode arch=compute_60,code=sm_60

./raytracing

pnmtopng images/image.ppm > images/image.png