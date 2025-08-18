#include "FileDialog.hpp"

#ifdef __APPLE__
    #include <Cocoa/Cocoa.h>
    #include <Foundation/Foundation.h>
    #include <AppKit/AppKit.h>
    #import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>


std::vector<std::filesystem::path> FileDialog::ShowFileDialog(bool isSave)
{
    std::vector<std::filesystem::path> paths;
    @autoreleasepool
    {
        // NSOpenPanel은 파일 열기 대화상자를 생성하고 관리하는 클래스입니다.
        NSOpenPanel *panel = [NSOpenPanel openPanel];

        // 여러 파일 선택 허용
        [panel setAllowsMultipleSelection:YES];
        // 디렉토리 선택 비허용
        [panel setCanChooseDirectories:NO];
        // 파일 선택 허용
        [panel setCanChooseFiles:YES];
        // 파일 확장자를 기준으로 선택 가능한 파일 제한
        // macOS 12+ : UTI 방식 사용
        UTType *objType = [UTType typeWithFilenameExtension:@"glb"];
        if (objType) {
            panel.allowedContentTypes = @[objType];
        }
        // 사용자가 "열기"를 누를 때까지 대화상자를 모달로 실행합니다.
        // NSModalResponseOK는 사용자가 "열기"를 클릭했음을 의미합니다.
        if ([panel runModal] == NSModalResponseOK)
        {
            // 선택된 모든 파일의 URL을 가져옵니다.
            for (NSURL *url in [panel URLs])
            {
                // NSURL 객체를 UTF-8 인코딩의 C++ 문자열(std::string)로 변환합니다.
                paths.push_back([url.path UTF8String]);
            }
        }
    }
    return paths;
}
#endif