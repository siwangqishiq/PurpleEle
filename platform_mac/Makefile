include ../module.mk

# ${BUILD_DIR}/audio_impl.o:${SRC_DIR}/audio/audio_mac.cpp \
# 				${SRC_DIR}/audio/audio_mac.hpp
# 	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/audio/audio_mac.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/audio_impl.o 


${BUILD_DIR}/main.o:main.cpp
	${CC} -std=${STD} -c main.cpp -o ${BUILD_DIR}/main.o -I ${INCLUDE_DIR} -I ../ -I ${SRC_DIR}
	
link:compile
	${CC} ${BUILD_DIR}/*.o -o ${BUILD_DIR}/main -Llib -lglfw3 -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
	
run:link
	${BUILD_DIR}/main

clean:
	rm -f ${BUILD_DIR}/*.o 
	rm -f ${BUILD_DIR}/main