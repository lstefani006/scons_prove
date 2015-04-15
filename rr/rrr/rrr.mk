##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=rrr
ConfigurationName      :=Debug
WorkspacePath          := "/home/leo/tmp/scons_prove/rr/rrr"
ProjectPath            := "/home/leo/tmp/scons_prove/rr/rrr"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=leo
Date                   :=09/07/2014
CodeLitePath           :="/home/leo/.codelite"
LinkerName             :=/usr/bin/clang++ 
SharedObjectLinkerName :=/usr/bin/clang++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="rrr.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/llvm-ar rcu
CXX      := /usr/bin/clang++ 
CC       := /usr/bin/clang 
CXXFLAGS :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/llvm-as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/rr_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_rr_lex.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_rr_tab.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/rr_c.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/rr_main.cpp$(ObjectSuffix): ../main.cpp $(IntermediateDirectory)/rr_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_main.cpp$(DependSuffix): ../main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_main.cpp$(DependSuffix) -MM "../main.cpp"

$(IntermediateDirectory)/rr_main.cpp$(PreprocessSuffix): ../main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_main.cpp$(PreprocessSuffix) "../main.cpp"

$(IntermediateDirectory)/rr_rr_lex.cpp$(ObjectSuffix): ../rr_lex.cpp $(IntermediateDirectory)/rr_rr_lex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/rr_lex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_rr_lex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_rr_lex.cpp$(DependSuffix): ../rr_lex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_rr_lex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_rr_lex.cpp$(DependSuffix) -MM "../rr_lex.cpp"

$(IntermediateDirectory)/rr_rr_lex.cpp$(PreprocessSuffix): ../rr_lex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_rr_lex.cpp$(PreprocessSuffix) "../rr_lex.cpp"

$(IntermediateDirectory)/rr_rr_tab.cpp$(ObjectSuffix): ../rr_tab.cpp $(IntermediateDirectory)/rr_rr_tab.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/rr_tab.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_rr_tab.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_rr_tab.cpp$(DependSuffix): ../rr_tab.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_rr_tab.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_rr_tab.cpp$(DependSuffix) -MM "../rr_tab.cpp"

$(IntermediateDirectory)/rr_rr_tab.cpp$(PreprocessSuffix): ../rr_tab.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_rr_tab.cpp$(PreprocessSuffix) "../rr_tab.cpp"

$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(ObjectSuffix): ../CheckType_DeclVisitor.cpp $(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/CheckType_DeclVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(DependSuffix): ../CheckType_DeclVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(DependSuffix) -MM "../CheckType_DeclVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(PreprocessSuffix): ../CheckType_DeclVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_CheckType_DeclVisitor.cpp$(PreprocessSuffix) "../CheckType_DeclVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(ObjectSuffix): ../CheckType_ExprVisitor.cpp $(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/CheckType_ExprVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(DependSuffix): ../CheckType_ExprVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(DependSuffix) -MM "../CheckType_ExprVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(PreprocessSuffix): ../CheckType_ExprVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_CheckType_ExprVisitor.cpp$(PreprocessSuffix) "../CheckType_ExprVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(ObjectSuffix): ../CheckType_StmtVisitor.cpp $(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/CheckType_StmtVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(DependSuffix): ../CheckType_StmtVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(DependSuffix) -MM "../CheckType_StmtVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(PreprocessSuffix): ../CheckType_StmtVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_CheckType_StmtVisitor.cpp$(PreprocessSuffix) "../CheckType_StmtVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(ObjectSuffix): ../CheckType_TypeVisitor.cpp $(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/CheckType_TypeVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(DependSuffix): ../CheckType_TypeVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(DependSuffix) -MM "../CheckType_TypeVisitor.cpp"

$(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(PreprocessSuffix): ../CheckType_TypeVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_CheckType_TypeVisitor.cpp$(PreprocessSuffix) "../CheckType_TypeVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(ObjectSuffix): ../Dump_DeclVisitor.cpp $(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/Dump_DeclVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(DependSuffix): ../Dump_DeclVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(DependSuffix) -MM "../Dump_DeclVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(PreprocessSuffix): ../Dump_DeclVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_Dump_DeclVisitor.cpp$(PreprocessSuffix) "../Dump_DeclVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(ObjectSuffix): ../Dump_ExprVisitor.cpp $(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/Dump_ExprVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(DependSuffix): ../Dump_ExprVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(DependSuffix) -MM "../Dump_ExprVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(PreprocessSuffix): ../Dump_ExprVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_Dump_ExprVisitor.cpp$(PreprocessSuffix) "../Dump_ExprVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(ObjectSuffix): ../Dump_StmtVisitor.cpp $(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/Dump_StmtVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(DependSuffix): ../Dump_StmtVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(DependSuffix) -MM "../Dump_StmtVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(PreprocessSuffix): ../Dump_StmtVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_Dump_StmtVisitor.cpp$(PreprocessSuffix) "../Dump_StmtVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(ObjectSuffix): ../Dump_TypeVisitor.cpp $(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/Dump_TypeVisitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(DependSuffix): ../Dump_TypeVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(DependSuffix) -MM "../Dump_TypeVisitor.cpp"

$(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(PreprocessSuffix): ../Dump_TypeVisitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_Dump_TypeVisitor.cpp$(PreprocessSuffix) "../Dump_TypeVisitor.cpp"

$(IntermediateDirectory)/rr_c.cpp$(ObjectSuffix): ../c.cpp $(IntermediateDirectory)/rr_c.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/leo/tmp/scons_prove/rr/c.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rr_c.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rr_c.cpp$(DependSuffix): ../c.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rr_c.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rr_c.cpp$(DependSuffix) -MM "../c.cpp"

$(IntermediateDirectory)/rr_c.cpp$(PreprocessSuffix): ../c.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rr_c.cpp$(PreprocessSuffix) "../c.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/rrr"


