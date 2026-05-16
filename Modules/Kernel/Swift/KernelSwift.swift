import XyrisKernelBridge

@_cdecl("KernelSwiftRun")
public func kernelSwiftRun() {
    xyris_loader_show_progress()
}
