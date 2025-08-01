# WebGPT


After six years of development, WebGPU is about to launch across most major web browsers. This is massive: web applications now have near-native access to the GPU, with the added capacity of compute shaders.

WebGPT is a vanilla JS and HTML implementation of a transformer model, intended as a proof-of-concept as well as educational resource. WebGPT has been tested to be working with models up to 500 M parameters, though could likely support far more with further testing/optimization.

## UnifiedAI Example

=======
=======
The repository also contains an example Python implementation of a modular AI system named **UnifiedAI**. See [`UnifiedAI.md`](UnifiedAI.md) for details and the `unified_ai` package for the source code. The example exposes `/query`, `/health`, and `/metrics` endpoints via FastAPI and includes a `NetworkFeatureManager` for optional networking capabilities.
=======
=======
The repository also contains an example Python implementation of a modular AI system named **UnifiedAI**. See [`UnifiedAI.md`](UnifiedAI.md) for details and run it with `python -m unified_ai`. The example exposes `/query`, `/health`, and `/metrics` endpoints via FastAPI and includes a `NetworkFeatureManager` for optional networking capabilities. Our guiding principles are outlined in [DECLARATION.md](DECLARATION.md).
=======
The repository also contains an example Python implementation of a modular AI system named **UnifiedAI**. See [`UnifiedAI.md`](UnifiedAI.md) for details and the `unified_ai` package for the source code. The example exposes `/query`, `/health`, and `/metrics` endpoints via FastAPI and includes a `NetworkFeatureManager` for optional networking capabilities. Our guiding principles are outlined in [DECLARATION.md](DECLARATION.md).

### Current Stats
2020 M1 Mac: 3ms/token at 5M parameters with f32 precision.  
2020 M1 Mac: 30ms/token at 117M parameters with f32 precision.  
2020 M1 Mac: 70ms/token at 377M parameters with f32 precision.
2020 M1 Mac: 120ms/token at 775M parameters with f32 precision.
1.5B is working but unstable, sitting around 1000ms/token due to inefficiencies.

### Running UnifiedAI with Docker

Use the provided `Dockerfile` and `docker-compose.yml` to start the API and a Redis instance:

```bash
docker-compose up --build
```

## Running WebGPT

Running WebGPT is remarkably simple, as it's just a set of HTML + JS files. Since WebGPU is still in the process of being released, you'll need to open with a compatible browser. WebGPU is currently available on Chrome v113 but the most straightforward way to ensure proper functionality is to install [Chrome Canary](https://www.google.com/chrome/canary/) or Edge Canary.

I've included two different models: a toy GPT-Shakespeare model (which is severly undertrained haha) and GPT-2 117M. See main.js for more information on how to run these models. If you want to import custom models, take a look at misc/conversion_scripts.

If you want to try out WebGPT, visit the demo website here [KMeans.org](https://www.kmeans.org). I'd generally reccomend cloning the repo and running locally, just because loading the weights remotely is significantly slower.  
Note: **You'll need to use Git LFS** to download the model files, after cloning the repository.


## Roadmap / Fixing Stupid Decisions

- [x] Embeddings / de-embeddings on GPU.
- [x] Initializing pipelines on every step is incredibly inefficient.
- [x] Key-value caching.
- [x] Reuse buffers.
- [x] Kernel shared memory for matmul!
- [x] Destroy buffers after use!
- [x] Create kernel instruction classes + optimize pipeline creation.
- [X] Fuse all kernels.
- [X] Optimize all other kernels.
- [X] Compute pass splitting for larger models _(maxStorageBufferBindingSize)_
- [ ] Run selection ops on GPU (topk, selection softmax)
- [ ] Attention kernel is optimized for small models, not for large models where each head having it's own matmul is more efficient.
- [ ] Investigate why attention cache isn't giving proper speed-ups.
- [ ] Make simple instructional version without special stuff.
- [ ] Optimize workgroup sizes, specifically for single row/col operations.
- [ ] Convert into a package.
- [ ] Write better comments + make Youtube explainer.

## Acknowledgements

When I started this project I had no idea how transformers worked or how to implement them (or GPUs or matmul kernels or WebGPU or tokenization for that matter), so Andrej Karpathy's series on neural networks and building GPT from scratch were invaluable: [Andrej's Youtube](https://www.youtube.com/@AndrejKarpathy). I've also used some code as well from the nanoGPT repository: [nanoGPT](https://github.com/karpathy/nanoGPT).

I copied from LatitudeGames' implementation of OpenAI's GPT-3 tokenizer in Javascript: [GPT-3-Encoder](https://github.com/latitudegames/GPT-3-Encoder).

## Running the SRE Chatbot

The repository includes a simple Tkinter interface demonstrating the Self-Reflective Entity (SRE) engine.

1. Install the Python requirements:

   ```bash
   pip install -r requirements.txt
   ```

2. Launch the chat GUI:

   ```bash
   python united_ai_gui.py
   ```

The chatbot analyzes the tone of your messages with TextBlob and replies with tone-aware responses. Archetype emergence events will be printed in the chat log.
## UnifiedAI Control Panel

Run `python united_ai_gui.py` to open the control panel and chat with UnifiedAI.


## BrainEngine

`BrainEngine` is a lightweight module for experimentation with higher level
cognition.  It exposes basic `reason`, `solve_problem` and memory management
helpers. Memories are kept in a simple dictionary and a configurable limit
prevents unbounded growth.  All actions are logged so you can extend the engine
with richer logic.

The updated implementation integrates an optional LLM backend for more advanced
reasoning and problem solving. Memories carry metadata such as timestamps,
access counts and tags which enables simple contextual retrieval. While the SRE
engine focuses on tone analysis and archetype emergence, BrainEngine can track
additional knowledge or context. The chat GUI instantiates both engines so you
can build on this example.

An example datasheet for an optical engine is loaded at startup to demonstrate
the memory system. You can find it in `datasheets/optical_engine_datasheet.json`.
The AuraEngine reads initial rules from `ethics_rules.json`, which is included here for convenience.

## Running the Modules and Tests

1. Install the Python requirements:
   ```bash
   pip install -r requirements.txt
   ```
2. Start the UnifiedAI API:
   ```bash
=======
    python -m unified_ai
=======
   python -m unified_ai
   ```
3. Launch the UnifiedAI GUI:
   ```bash
   python united_ai_gui.py
   ```
4. Execute the test suite:
   ```bash
   pytest
   ```
=======
=======
=======
=======

## Running Tests

Install dependencies first:

```bash
pip install -r requirements.txt
```

Then execute the test suite with:

```bash
pytest
```

If you have development packages installed via `requirements-dev.txt`, ensure they are installed before running the tests.
