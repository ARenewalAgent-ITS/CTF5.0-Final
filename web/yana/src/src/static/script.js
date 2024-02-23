function getNotes() {
    return JSON.parse(localStorage.getItem('notes')) || [];
}

function saveNotes(notes) {
    localStorage.setItem('notes', JSON.stringify(notes));
}

function setNote(note) {
    const li = document.createElement('li');
    const div = document.createElement('div');
    const id = genToken();
    div.id = id;
    div.classList.add('bg-gray-200', 'p-2', 'rounded-md');
    li.classList.add('mb-2');
    li.appendChild(div);
    notesList.appendChild(li);

    const noteDiv = document.getElementById(id);
    const clean = DOMPurify.sanitize(note);
    noteDiv.innerHTML = clean;
}

function resetNotes() {
    localStorage.removeItem('notes');
    notesList.innerHTML = '';
}

function exportNotes() {
    const notes = getNotes();
    const data = btoa(JSON.stringify(notes));
    
    const url = `${window.location}?import=${encodeURIComponent(data)}`;
    navigator.clipboard.writeText(url);
}

function genToken() {
    const token = Math.random().toString(36).substring(2, 15) + Math.random().toString(36).substring(2, 15);
    return token;
}

const note = document.getElementById('note');
const saveNote = document.getElementById('submit');
const notesList = document.getElementById('notes-list');
const reset = document.getElementById('reset');
const exportBtn = document.getElementById('export');

saveNote.addEventListener('click', () => {
    setNote(note.value);

    const notes = getNotes() || [];
    notes.push(note.value);
    saveNotes(notes);
});

reset.addEventListener('click', () => {
    resetNotes();
});

exportBtn.addEventListener('click', () => {
    exportNotes();
});

if (window.location.search.includes('import')) {
    try {
        const data = window.location.search.split('import=')[1];
        const notes = JSON.parse(atob(decodeURIComponent(data)));
        notes.forEach(note => {
            setNote(note);
        });
        saveNotes(notes);
    }
    catch (e) {
        console.error(e);
    }
} else {
    const notes = getNotes() || [];
    notes.forEach(note => {
        setNote(note);
    });
}