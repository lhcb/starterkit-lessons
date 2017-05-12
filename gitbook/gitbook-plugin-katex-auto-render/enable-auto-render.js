function renderMathInBody() {
  // This will be rendered in display mode:
  //   $$
  //   x = y
  //   $$
  // This will be rendered in inline mode:
  //   $$x = y$$
  var katexOpts = {
    delimiters: [
      {left: "$$\n", right: "$$", display: true},
      {left: "$$", right: "$$", display: false}
    ]
  };
  window.renderMathInElement(document.body, katexOpts);
}

// Render math when the page is loaded and on subsequent navigations
// (navigations change the content of the DOM)
document.addEventListener("DOMContentLoaded", renderMathInBody);
require(["gitbook"], function(gitbook) {
  gitbook.events.bind("page.change", renderMathInBody);
});
