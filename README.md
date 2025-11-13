# helm-chart-in-cpp

> [!NOTE]
> Disclaimer: Major parts of this project were written with AI/LLMs, notably
> Anthropic Claude and OpenCode

With the new [Helm v4
release's](https://github.com/helm/helm/releases/tag/v4.0.0) capability to
implement plugins with WASM I thought it would be funny to implement a "Helm
chart" in C++. Now technically this isn't a "Helm chart" it's a postrender
plugin and requires a seed chart but it technically should work.

But it doesn't work. Because postrender WASM plugins are borked in Helm 4 right
now. But once that is fixed this should work.

and yes i vibe coded the makefile and base64 and minimal libc functions sorry

## Installation

lol

## Usage

```shell
$ helm template example examples/simple-chart/ --post-renderer cpp-renderer
Error: error while running post render on files: failed to invoke post-renderer plugin "cpp-renderer": failed to json marshal plugin output message: reflect.Value: json: can
not unmarshal string into Go struct field OutputMessagePostRendererV1.manifests of type bytes.Buffer
```
