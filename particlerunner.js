let initialize_particle_system = Module.cwrap("initialize_particle_system", null, ["number", "number", "number"]);
let update_particle_system = Module.cwrap("update_particle_system", null, ["number", "number", "number"]);
let n = 100;
let colors_array, positions_array;
const start = async () => {
    // Instantiate the wasm.
    // const res = await WebAssembly.instantiate(buf, {})
  
    // // Get the function out of the exports.Module.HEAP32.buffer
    // const { sumArrayInt32, memory } = res.instance.exports
    var colors_ptr = Module._malloc(4*n*3);
    var positions_ptr = Module._malloc(4*n*2);
    colors_array = new Float32Array(Module.HEAP32.buffer, colors_ptr, n*3);
    positions_array = new Float32Array(Module.HEAP32.buffer, positions_ptr, n*2);

    initialize_particle_system(n, colors_array.byteOffset, positions_array.byteOffset);
    addParticles();
}

const showArrays = () => {
    console.log(colors_array);
    console.log(positions_array);
}

var mousePos;
document.onmousemove = handleMouseMove;
function handleMouseMove(event) {
    var dot, eventDoc, doc, body, pageX, pageY;

    event = event || window.event; // IE-ism

    // If pageX/Y aren't available and clientX/Y are,
    // calculate pageX/Y - logic taken from jQuery.
    // (This is to support old IE)
    if (event.pageX == null && event.clientX != null) {
        eventDoc = (event.target && event.target.ownerDocument) || document;
        doc = eventDoc.documentElement;
        body = eventDoc.body;

        event.pageX = event.clientX +
          (doc && doc.scrollLeft || body && body.scrollLeft || 0) -
          (doc && doc.clientLeft || body && body.clientLeft || 0);
        event.pageY = event.clientY +
          (doc && doc.scrollTop  || body && body.scrollTop  || 0) -
          (doc && doc.clientTop  || body && body.clientTop  || 0 );
    }

    mousePos = {
        x: event.pageX,
        y: event.pageY
    };
}

const addParticles = () => {
    let particles = document.getElementsByClassName("particles")
    Array.from(particles).map((val) => {
        val.remove();
    })
    Array.from(Array(n).keys()).map((index)=>{
        const r = colors_array[index*3];
        const g = colors_array[index*3 + 1];
        const b = colors_array[index*3 + 2];
        const div = document.createElement("div");
        div.id = "particle_"+index.toString();
        div.className = "particles";
        div.style.borderRadius = "50%";
        div.style.backgroundColor = "rgb(" + r.toString() + ", " + g.toString() + ", " + b.toString() + ")";
        div.style.position = "absolute";
        div.style.height = "5px";
        div.style.width = "5px";
        div.style.left = positions_array[index*2];
        div.style.top = positions_array[index*2 + 1];
        document.body.appendChild(div);
    })
    updateParticles(0)
}

const updateParticles = async (delta) => {
    // console.log("delta", delta);
    // console.log(mousePos?.x, mousePos?.y)
    if(mousePos)
    {
        await update_particle_system(mousePos.x, mousePos.y, delta);
        Array.from(Array(n).keys()).map((index)=>{
            const div = document.getElementById("particle_"+index.toString());
            const r = colors_array[index*3];
            const g = colors_array[index*3 + 1];
            const b = colors_array[index*3 + 2];
            div.style.backgroundColor = "rgb(" + r.toString() + ", " + g.toString() + ", " + b.toString() + ")";
            div.style.left = positions_array[index*2];
            div.style.top = positions_array[index*2 + 1];
        })
    }
    

    requestAnimationFrame(updateParticles)
}