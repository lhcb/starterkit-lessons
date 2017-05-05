document.addEventListener("DOMContentLoaded", function() {
  var katexOpts = {
    delimiters: [
      {left: "$$", right: "$$", display: false},
      {left: "\\[", right: "\\]", display: true},
      {left: "\\(", right: "\\)", display: false}
    ]
  };
  window.renderMathInElement(document.body, katexOpts);
});
