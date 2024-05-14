const spinner = document.getElementById('spinner')

function showSpinner() {
    spinner.style.display = 'block'
}

function hideSpinner() {
    spinner.style.display = 'none'
}

export { showSpinner, hideSpinner }
