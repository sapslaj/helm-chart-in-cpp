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

HOWEVER!!! if you add a custom `UnmarshalJSON` implementation to the `OutputMessagePostRendererV1` struct, then it magically fixes it.

```go
// internal/plugin/schema/postrenderer.go
func (o *OutputMessagePostRendererV1) UnmarshalJSON(data []byte) error {
	type real struct {
		Manifests []byte `json:"manifests"`
	}
	var res real
	err := json.Unmarshal(data, &res)
	if err != nil {
		return err
	}
	o.Manifests = bytes.NewBuffer(res.Manifests)
	return nil
}
```

and yes i vibe coded the makefile and base64 and minimal libc functions sorry

## Installation

lol

## Usage

```shell
$ helm template example examples/simple-chart/ --post-renderer cpp-renderer
---
# Source: generated-by-postrender-0.yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx
  labels:
    app: nginx
spec:
  replicas:
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:latest
        ports:
        - containerPort: 80
```
