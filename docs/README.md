# VneScene Documentation

Documentation for the VneScene library (cameras, lights, and scene state for VertexNova).

## Generating API Documentation

Generate API documentation with Doxygen:

```bash
cmake -DENABLE_DOXYGEN=ON -B build
cmake --build build --target vnescene_doc_doxygen
```

Documentation will be available at `build/docs/html/index.html`.

Alternatively, use the script (if available):

```bash
./scripts/generate-docs.sh --api-only
```

## Requirements

- Doxygen 1.8.13 or higher
- Graphviz (for class and call graphs)
