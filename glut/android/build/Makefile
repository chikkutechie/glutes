
lib_PROGRAM=../libs/armeabi/ligglutes.so
NDB_BUILD = $(ANDROID_NDK_ROOT)/ndk-build

all: $(lib_PROGRAM)

$(lib_PROGRAM):
	cd ../jni && $(NDB_BUILD)

clean:
	$(NDB_BUILD) clean
