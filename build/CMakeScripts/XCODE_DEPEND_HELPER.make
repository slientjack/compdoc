# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.cfbf.Debug:
/Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/Debug${EFFECTIVE_PLATFORM_NAME}/libcfbf.a:
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/Debug${EFFECTIVE_PLATFORM_NAME}/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Debug/cfbf.build/$(OBJDIR)/armv7/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Debug/cfbf.build/$(OBJDIR)/armv7s/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Debug/cfbf.build/$(OBJDIR)/arm64/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Debug/cfbf.build/$(OBJDIR)/i386/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Debug/cfbf.build/$(OBJDIR)/x86_64/libcfbf.a


PostBuild.cfbf.Release:
/Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/Release${EFFECTIVE_PLATFORM_NAME}/libcfbf.a:
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/Release${EFFECTIVE_PLATFORM_NAME}/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Release/cfbf.build/$(OBJDIR)/armv7/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Release/cfbf.build/$(OBJDIR)/armv7s/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Release/cfbf.build/$(OBJDIR)/arm64/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Release/cfbf.build/$(OBJDIR)/i386/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/Release/cfbf.build/$(OBJDIR)/x86_64/libcfbf.a


PostBuild.cfbf.MinSizeRel:
/Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libcfbf.a:
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/MinSizeRel/cfbf.build/$(OBJDIR)/armv7/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/MinSizeRel/cfbf.build/$(OBJDIR)/armv7s/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/MinSizeRel/cfbf.build/$(OBJDIR)/arm64/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/MinSizeRel/cfbf.build/$(OBJDIR)/i386/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/MinSizeRel/cfbf.build/$(OBJDIR)/x86_64/libcfbf.a


PostBuild.cfbf.RelWithDebInfo:
/Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libcfbf.a:
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/RelWithDebInfo/cfbf.build/$(OBJDIR)/armv7/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/RelWithDebInfo/cfbf.build/$(OBJDIR)/armv7s/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/RelWithDebInfo/cfbf.build/$(OBJDIR)/arm64/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/RelWithDebInfo/cfbf.build/$(OBJDIR)/i386/libcfbf.a
	/bin/rm -f /Users/yejianfei/Sources/dofunc/cpp/ms-composite-doc/build/cfbf.build/RelWithDebInfo/cfbf.build/$(OBJDIR)/x86_64/libcfbf.a




# For each target create a dummy ruleso the target does not have to exist
