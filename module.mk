CC := g++

DIR := ..
SRC_DIR := ../src
ASSET_DIR := ../assets
WIN_DIR := .
BUILD_DIR := build
INCLUDE_DIR := ../include
STD := c++14
# STD := gnu++14
OPTPARAMS := -O1

.PHONY: clean

build_dir:
	mkdir -p ${BUILD_DIR}

compile: build_dir \
		${BUILD_DIR}/main.o \
		${BUILD_DIR}/application.o \
		${BUILD_DIR}/json.o \
		${BUILD_DIR}/utils.o \
		${BUILD_DIR}/glad.o \
		${BUILD_DIR}/shader.o \
		${BUILD_DIR}/triangle.o \
		${BUILD_DIR}/render.o \
		${BUILD_DIR}/render_batch.o \
		${BUILD_DIR}/command.o \
		${BUILD_DIR}/vram.o \
		${BUILD_DIR}/asset_manager.o \
		${BUILD_DIR}/texture.o \
		${BUILD_DIR}/view.o \
		${BUILD_DIR}/drawable.o \
		${BUILD_DIR}/timer.o \
		${BUILD_DIR}/sprite.o \
		${BUILD_DIR}/audio_manager.o \
		${BUILD_DIR}/test_demo.o \
		${BUILD_DIR}/shader_demo.o \
		${BUILD_DIR}/ui_demo.o \
		${BUILD_DIR}/counter_demo.o \
		${BUILD_DIR}/life_game.o \
		${BUILD_DIR}/ninjia_game.o \
		${BUILD_DIR}/custom_shader_demo.o \

${BUILD_DIR}/json.o: ${DIR}/libjson/json.cpp ${DIR}/libjson/json.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${DIR}/libjson/json.cpp -o ${BUILD_DIR}/json.o

${BUILD_DIR}/utils.o: ${SRC_DIR}/utils.cpp ${SRC_DIR}/utils.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/utils.cpp -I ${INCLUDE_DIR} -o ${BUILD_DIR}/utils.o

${BUILD_DIR}/application.o: ${SRC_DIR}/application.hpp ${SRC_DIR}/application.cpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/application.cpp -o ${BUILD_DIR}/application.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/shader.o:${SRC_DIR}/render/shader.cpp ${SRC_DIR}/render/shader.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/shader.cpp -o ${BUILD_DIR}/shader.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/command.o:${SRC_DIR}/render/command.cpp ${SRC_DIR}/render/command.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/command.cpp -o ${BUILD_DIR}/command.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/triangle.o:${SRC_DIR}/render/triangle.cpp \
						${SRC_DIR}/render/triangle.hpp 
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/triangle.cpp -o ${BUILD_DIR}/triangle.o -I ${INCLUDE_DIR} -I ${SRC_DIR} 

${BUILD_DIR}/timer.o:${SRC_DIR}/widget/timer.cpp \
						${SRC_DIR}/widget/timer.hpp 
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/widget/timer.cpp -o ${BUILD_DIR}/timer.o -I ${INCLUDE_DIR} -I ${SRC_DIR} 

${BUILD_DIR}/vram.o:${SRC_DIR}/render/vram.cpp ${SRC_DIR}/render/vram.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/vram.cpp -o ${BUILD_DIR}/vram.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/render.o:${SRC_DIR}/render/render.cpp ${SRC_DIR}/render/render.hpp 
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/render.cpp -o ${BUILD_DIR}/render.o -I ${INCLUDE_DIR} -I ${SRC_DIR} 

${BUILD_DIR}/render_batch.o:${SRC_DIR}/render/render_batch.cpp \
						${SRC_DIR}/render/render_batch.hpp 
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/render_batch.cpp -o ${BUILD_DIR}/render_batch.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/texture.o:${SRC_DIR}/render/texture.cpp ${SRC_DIR}/render/texture.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/texture.cpp -o ${BUILD_DIR}/texture.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/sprite.o:${SRC_DIR}/render/sprite.cpp ${SRC_DIR}/render/sprite.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/sprite.cpp -o ${BUILD_DIR}/sprite.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/asset_manager.o:${SRC_DIR}/resource/asset_manager.cpp ${SRC_DIR}/resource/asset_manager.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/resource/asset_manager.cpp -o ${BUILD_DIR}/asset_manager.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/audio_manager.o:${SRC_DIR}/audio/audio.cpp \
						${SRC_DIR}/audio/audio.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/audio/audio.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/audio_manager.o 

${BUILD_DIR}/view.o:${SRC_DIR}/render/ui/view.cpp \
						${SRC_DIR}/render/ui/view.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/ui/view.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/view.o 

${BUILD_DIR}/drawable.o:${SRC_DIR}/render/ui/drawable.cpp \
						${SRC_DIR}/render/ui/drawable.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/render/ui/drawable.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/drawable.o 

${BUILD_DIR}/test_demo.o:${SRC_DIR}/game/test_demo.cpp ${SRC_DIR}/game/test_demo.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/test_demo.cpp -o ${BUILD_DIR}/test_demo.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/shader_demo.o:${SRC_DIR}/game/shader_demo.cpp ${SRC_DIR}/game/shader_demo.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/shader_demo.cpp -o ${BUILD_DIR}/shader_demo.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/ui_demo.o:${SRC_DIR}/game/ui_demo.cpp ${SRC_DIR}/game/ui_demo.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/ui_demo.cpp -o ${BUILD_DIR}/ui_demo.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/counter_demo.o:${SRC_DIR}/game/counter_demo.cpp ${SRC_DIR}/game/counter_demo.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/counter_demo.cpp -o ${BUILD_DIR}/counter_demo.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/life_game.o:${SRC_DIR}/game/life_game.cpp ${SRC_DIR}/game/life_game.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/life_game.cpp -o ${BUILD_DIR}/life_game.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/custom_shader_demo.o:${SRC_DIR}/game/custom_shader_demo.cpp ${SRC_DIR}/game/custom_shader_demo.hpp
	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/game/custom_shader_demo.cpp -o ${BUILD_DIR}/custom_shader_demo.o -I ${INCLUDE_DIR} -I ${SRC_DIR}

${BUILD_DIR}/ninjia_game.o: ${SRC_DIR}/game/ninjia/ninjia_game_base.hpp \
		${SRC_DIR}/game/ninjia/ninjia_game.hpp \
		${SRC_DIR}/game/ninjia/ninjia_game.cpp \
		${BUILD_DIR}/ninjia_sprite.o \
		${BUILD_DIR}/ninjia_background.o
	${CC} -std=${STD} -c ${OPTPARAMS} -I ${INCLUDE_DIR} -I ${SRC_DIR} ${SRC_DIR}/game/ninjia/ninjia_game.cpp -o ${BUILD_DIR}/ninjia_game.o

${BUILD_DIR}/ninjia_sprite.o: ${SRC_DIR}/game/ninjia/ninjia_game_base.hpp \
		${SRC_DIR}/game/ninjia/ninjia_sprite.hpp \
		${SRC_DIR}/game/ninjia/ninjia_sprite.cpp 
	${CC} -std=${STD} -c ${OPTPARAMS} -I ${INCLUDE_DIR} -I ${SRC_DIR} ${SRC_DIR}/game/ninjia/ninjia_sprite.cpp -o ${BUILD_DIR}/ninjia_sprite.o

${BUILD_DIR}/ninjia_background.o: ${SRC_DIR}/game/ninjia/ninjia_game_base.hpp \
		${SRC_DIR}/game/ninjia/ninjia_background.hpp \
		${SRC_DIR}/game/ninjia/ninjia_background.cpp 
	${CC} -std=${STD} -c ${OPTPARAMS} -I ${INCLUDE_DIR} -I ${SRC_DIR} ${SRC_DIR}/game/ninjia/ninjia_background.cpp -o ${BUILD_DIR}/ninjia_background.o
		

# ${BUILD_DIR}/audio_impl.o:${SRC_DIR}/audio/audio_impl.cpp \
# 				${SRC_DIR}/audio/audio_impl.hpp
# 	${CC} -std=${STD} -c ${OPTPARAMS} ${SRC_DIR}/audio/audio_impl.cpp -I ${INCLUDE_DIR} -I ${SRC_DIR} -o ${BUILD_DIR}/audio_impl.o 