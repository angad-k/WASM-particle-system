var js_wrapped_initialize_particle_system = Module.cwrap("initialize_particle_system", null, ["number", "number", "number"]);

const start = () => {
    var n = 10;
    var colors = Module._malloc(4 * n * 3);
    var positions = Module._malloc(4 * n * 2);

    js_wrapped_initialize_particle_system(n, colors, positions);
    console.log("reached")
}