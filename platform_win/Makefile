include ../module.mk

${BUILD_DIR}/audio_impl.o:${SRC_DIR}/audio/audio_windows.cpp \
				${SRC_DIR}/audio/audio_windows.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/audio/audio_windows.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/audio_impl.o 


${BUILD_DIR}/main.o:main.cpp
	${CC} -std=${STD} ${OPTPARAMS} -c main.cpp -o ${BUILD_DIR}/main.o -I ${INCLUDE_DIR} -I ../ -I ${SRC_DIR}
	
link:compile
	${CC} ${BUILD_DIR}/*.o -o ${BUILD_DIR}/main.exe -Llib -lglfw3dll
	
run:link
	${BUILD_DIR}/main.exe

#test
# asset_test_run:	${SRC_DIR}/resource/asset_manager.cpp \
# 				${SRC_DIR}/resource/asset_manager.hpp \
# 				${SRC_DIR}/resource/asset_manager_test.cpp
# 	${CC} -std=${STD} ${SRC_DIR}/resource/asset_manager.cpp ${SRC_DIR}/resource/asset_manager_test.cpp -o ${BUILD_DIR}/asset_test_run -I ${INCLUDE_DIR} -I ${SRC_DIR}

# test_asset:asset_test_run
# 	${BUILD_DIR}/asset_test_run

clean:
	rm -f ${BUILD_DIR}/*.o 
	rm -f ${BUILD_DIR}/*.exe
	rm -f ${BUILD_DIR}/main.exe